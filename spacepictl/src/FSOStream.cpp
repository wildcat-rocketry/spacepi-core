#include <cstddef>
#include <cstdio>
#include <exception>
#include <memory>
#include <ostream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/FSOperation.hpp>
#include <spacepi/spacepictl/FSOStream.hpp>
#include <spacepi/spacepictl/FSTransaction.hpp>

using namespace std;
using namespace spacepi::util;
using namespace spacepi::spacepictl;
using namespace spacepi::spacepictl::detail;

FSOStreamBuf::FSOStreamBuf(const string &filename, uid_t uid, gid_t gid, mode_t mode, bool keepBackup) : filename(filename), uid(uid), gid(gid), mode(mode), keepBackup(keepBackup), hasBackup(false), wrote(false) {
}

void FSOStreamBuf::perform() {
    string tempName;
    int fd;
    if (access(filename.c_str(), W_OK) >= 0) {
        hasBackup = true;
        tempName = filename + "$";
        handle("opening temporary file to write", fd = open(tempName.c_str(), O_WRONLY | O_CREAT, mode));
    } else {
        handle("opening file to write", fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_EXCL, mode));
    }
    try {
        handle("setting file owner", fchown(fd, uid, gid));
        string data = str();
        const char *cdata = data.data();
        size_t size = data.size();
        while (size > 0) {
            ssize_t count;
            handle("writing to file", count = write(fd, cdata, size));
            if (count == 0) {
                throw EXCEPTION(ResourceException("File could not be written"));
            }
            cdata += count;
            size -= count;
        }
    } catch (const std::exception &ex) {
        close(fd);
        throw;
    }
    handle("closing file", close(fd));
    if (hasBackup) {
        string backupName = filename + "~";
        handle("creating backup file", rename(filename.c_str(), backupName.c_str()));
        wrote = true;
        handle("applying file changes", rename(tempName.c_str(), filename.c_str()));
    }
}

void FSOStreamBuf::undo() {
    if (hasBackup) {
        if (wrote) {
            string backupName = filename + "~";
            handle("reverting file change", rename(backupName.c_str(), filename.c_str()));
        }
    } else {
        handle("removing new file", unlink(filename.c_str()));
    }
}

void FSOStreamBuf::finalize() {
    if (hasBackup && !keepBackup) {
        string backupName = filename + "~";
        handle("removing backup file", unlink(backupName.c_str()));
    }
}

FSOStream::FSOStream(FSTransaction &transaction, const string &filename, uid_t uid, gid_t gid, mode_t mode, bool keepBackup) : ostream(nullptr) {
    shared_ptr<FSOStreamBuf> buf(new FSOStreamBuf(filename, uid, gid, mode, keepBackup));
    init(buf.get());
    transaction += buf;
}
