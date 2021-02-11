#include <cerrno>
#include <cstdio>
#include <exception>
#include <map>
#include <memory>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <git2.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/FSOperation.hpp>
#include <spacepi/spacepictl/FSTransaction.hpp>

using namespace std;
using namespace spacepi::log;
using namespace spacepi::util;
using namespace spacepi::spacepictl;
using namespace spacepi::spacepictl::detail;

FSGitConfigOperation::FSGitConfigOperation(const string &path, const map<string, string> &confEntries, uid_t uid, gid_t gid, mode_t mode) : path(path), confEntries(confEntries), uid(uid), gid(gid), mode(mode), hasBackup(false), wrote(false) {
}

void FSGitConfigOperation::git_handle(const std::string &msg, int res){
    if (res != 0) {
        string error_msg;
        const git_error *error = giterr_last();
        if(error != NULL){
            error_msg = error->message;
        } else {
            error_msg = strerror(errno);
        }
        throw EXCEPTION(ResourceException("Error " + msg + ": " + error_msg + ", res: " + to_string(res)));
    }    
}

void FSGitConfigOperation::perform() {
    string tempName;
    if (access(path.c_str(), W_OK) >= 0) {
        hasBackup = true;
        tempName = path + "$";
        FSTransaction fs;
        fs.copy(path, tempName, uid, gid, mode);
        fs.apply();
    } else {
        tempName = path;
        handle("making blank file", creat(tempName.c_str(), mode));
    }

    handle("changing owner of temp", chown(tempName.c_str(), uid, gid));

    git_config *new_config;
    git_handle("opening git config", git_config_open_ondisk(&new_config, tempName.c_str()));

    for(auto &i : confEntries){
        git_handle("adding git config entry", git_config_set_string(new_config, i.first.c_str(), i.second.c_str()));
    }

    git_config_free(new_config);

    if (hasBackup) {
        string backupName = path + "~";
        handle("creating backup file", rename(path.c_str(), backupName.c_str()));
        wrote = true;
        handle("applying file changes", rename(tempName.c_str(), path.c_str()));
    }
}

void FSGitConfigOperation::undo() {
    if (hasBackup) {
        if (wrote) {
            string backupName = path + "~";
            handle("reverting file change", rename(backupName.c_str(), path.c_str()));
        }
    } else {
        handle("removing new file", unlink(path.c_str()));
    }
}

void FSGitConfigOperation::finalize() {
    if (hasBackup) {
        string backupName = path + "~";
        handle("removing backup file", unlink(backupName.c_str()));
    }
}

FSMkSymlinkOperation::FSMkSymlinkOperation(const string &path, const string &target, uid_t uid, gid_t gid, mode_t mode) : path(path), target(target), uid(uid), gid(gid), mode(mode), hasBackup(false), wrote(false) {
}

void FSMkSymlinkOperation::perform() {
    string tempName;
    if (access(path.c_str(), W_OK) >= 0) {
        hasBackup = true;
        tempName = path + "$";
    } else {
        tempName = path;
    }

    handle("creating symlink", symlink(target.c_str(), tempName.c_str()));
    handle("setting file owner", chown(tempName.c_str(), uid, gid));
    handle("setting file mode", chmod(tempName.c_str(), mode));

    if (hasBackup) {
        string backupName = path + "~";
        handle("creating backup file", rename(path.c_str(), backupName.c_str()));
        wrote = true;
        handle("applying file changes", rename(tempName.c_str(), path.c_str()));
    }
}

void FSMkSymlinkOperation::undo() {
    if (hasBackup) {
        if (wrote) {
            string backupName = path + "~";
            handle("reverting file change", rename(backupName.c_str(), path.c_str()));
        }
    } else {
        handle("removing new file", unlink(path.c_str()));
    }
}

void FSMkSymlinkOperation::finalize() {
    if (hasBackup) {
        string backupName = path + "~";
        handle("removing backup file", unlink(backupName.c_str()));
    }
}

FSRemoveOperation::FSRemoveOperation(const string &file) : file(file), didExist(false) {
}

void FSRemoveOperation::perform() {
    if (access(file.c_str(), F_OK) >= 0) {
        didExist = true;
        string backupName = file + "~";
        handle("beginning to delete file", rename(file.c_str(), backupName.c_str()));
    }
}

void FSRemoveOperation::undo() {
    if (didExist) {
        string backupName = file + "~";
        handle("restoring deleted file", rename(backupName.c_str(), file.c_str()));
    }
}

void FSRemoveOperation::finalize() {
    if (didExist) {
        string backupName = file + "~";
        handle("deleting backup file", unlink(backupName.c_str()));
    }
}

FSMkdirOperation::FSMkdirOperation(const string &path, uid_t uid, gid_t gid, mode_t mode) : path(path), uid(uid), gid(gid), mode(mode), made(false), setOwn(false), setMod(false) {
}

void FSMkdirOperation::perform() {
    if (access(path.c_str(), F_OK) >= 0) {
        struct stat s;
        handle("stat'ing new directory", stat(path.c_str(), &s));
        if ((s.st_mode & S_IFMT) != S_IFDIR) {
            throw EXCEPTION(ResourceException("Cannot create directory in the place of a file"));
        }
        oldUid = s.st_uid;
        oldGid = s.st_gid;
        oldMode = (s.st_mode & 0777);
        if (oldUid != uid || oldGid != gid) {
            setOwn = true;
            handle("changing directory owner", chown(path.c_str(), uid, gid));
        }
        if (oldMode != mode) {
            setMod = true;
            handle("changing directory mode", chmod(path.c_str(), mode));
        }
    } else {
        handle("making new directory", mkdir(path.c_str(), mode));
        made = true;
        handle("changing new directory owner", chown(path.c_str(), uid, gid));
    }
}

void FSMkdirOperation::undo() {
    if (made) {
        handle("removing directory", rmdir(path.c_str()));
    } else {
        try {
            if (setOwn) {
                handle("restoring directory owner", chown(path.c_str(), oldUid, oldGid));
            }
        } catch (const std::exception &ex) {
            if (setMod) {
                handle("restoring directory mode", chmod(path.c_str(), oldMode));
            }
            throw;
        }
        if (setMod) {
            handle("restoring directory mode", chmod(path.c_str(), oldMode));
        }
    }
}

void FSMkdirOperation::finalize() {
}

FSCopyOperation::FSCopyOperation(const string &from, const string &to, uid_t uid, gid_t gid, mode_t mode) : from(from), to(to), uid(uid), gid(gid), mode(mode), hasBackup(false), wrote(false) {
}

void FSCopyOperation::perform() {
    int rfd;
    handle("opening original file", rfd = open(from.c_str(), O_RDONLY));
    string tempName;
    try {
        int wfd;
        if (access(to.c_str(), W_OK) >= 0) {
            hasBackup = true;
            tempName = to + "$";
            handle("opening temporary file to write", wfd = open(tempName.c_str(), O_WRONLY | O_CREAT, mode));
        } else {
            handle("opening file to write", wfd = open(to.c_str(), O_WRONLY | O_CREAT | O_EXCL, mode));
        }
        try {
            handle("setting file owner", fchown(wfd, uid, gid));
            char data[BUFSIZ];
            ssize_t count;
            do {
                handle("reading from file", count = read(rfd, data, BUFSIZ));
                char *dit = data;
                ssize_t left = count;
                while (left > 0) {
                    ssize_t wrote;
                    handle("writing to file", wrote = write(wfd, dit, left));
                    if (wrote == 0) {
                        throw EXCEPTION(ResourceException("File could not be written"));
                    }
                    dit += wrote;
                    left -= wrote;
                }
            } while (count > 0);
        } catch (const std::exception &ex) {
            close(wfd);
            throw;
        }
        handle("closing file", close(wfd));
    } catch (const std::exception &ex) {
        close(rfd);
        throw;
    }
    handle("closing file", close(rfd));
    if (hasBackup) {
        string backupName = to + "~";
        handle("creating backup file", rename(to.c_str(), backupName.c_str()));
        wrote = true;
        handle("applying file changes", rename(tempName.c_str(), to.c_str()));
    }
}

void FSCopyOperation::undo() {
    if (hasBackup) {
        if (wrote) {
            string backupName = to + "~";
            handle("reverting file change", rename(backupName.c_str(), to.c_str()));
        }
    } else {
        handle("removing new file", unlink(to.c_str()));
    }
}

void FSCopyOperation::finalize() {
    if (hasBackup) {
        string backupName = to + "~";
        handle("removing backup file", unlink(backupName.c_str()));
    }
}

FSTransaction::FSTransaction() noexcept : applied(false) {
}

void FSTransaction::apply() {
    if (applied) {
        throw EXCEPTION(StateException("Cannot apply filesystem transaction multiple times."));
    } else {
        applied = true;
    }
    vector<shared_ptr<FSOperation>>::iterator it;
    try {
        for (it = ops.begin(); it != ops.end(); ++it) {
            (*it)->perform();
        }
        for (it = ops.begin(); it != ops.end(); ++it) {
            try {
                (*it)->finalize();
            } catch (const exception &ex) {
                log(LogLevel::Warning) << "Error while finalizing transaction: " << ex.what() << "\n" << Exception::getPointer();
            }
        }
    } catch (const exception &ex) {
        log(LogLevel::Warning) << "Error performing transaction: " << ex.what() << "\n" << Exception::getPointer();
        for (; it >= ops.begin(); --it) {
            try {
                (*it)->undo();
            } catch (const exception &e) {
                log(LogLevel::Error) << "Error while rolling back transaction: " << e.what() << "\n" << Exception::getPointer();
            }
        }
        throw;
    }
}

FSTransaction &FSTransaction::operator +=(const shared_ptr<FSOperation> &op) {
    if (applied) {
        throw EXCEPTION(StateException("Cannot add to a transaction which has been applied."));
    }
    ops.push_back(op);
    return *this;
}

void FSTransaction::remove(const string &file) {
    *this += make_shared<FSRemoveOperation>(file);
}

void FSTransaction::mkdir(const string &path, uid_t uid, gid_t gid, mode_t mode) {
    *this += make_shared<FSMkdirOperation>(path, uid, gid, mode);
}

void FSTransaction::copy(const string &from, const string &to, uid_t uid, gid_t gid, mode_t mode) {
    *this += make_shared<FSCopyOperation>(from, to, uid, gid, mode);
}

void FSTransaction::link(const string &path, const string &target, uid_t uid, gid_t gid, mode_t mode){
    *this += make_shared<FSMkSymlinkOperation>(path, target, uid, gid, mode);
}

void FSTransaction::add_git_config(const string &path, map<string,string> confEntries, uid_t uid, gid_t gid, mode_t mode){
    *this += make_shared<FSGitConfigOperation>(path, confEntries, uid, gid, mode);
}
