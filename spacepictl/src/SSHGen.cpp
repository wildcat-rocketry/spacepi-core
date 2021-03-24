#include <cerrno>
#include <cstring>
#include <ostream>
#include <string>
#include <vector>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <pwd.h>
#include <sched.h>
#include <unistd.h>
#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/SSHGen.hpp>
#include <spacepi/spacepictl/Verb.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace boost::process;
using namespace spacepi::log;
using namespace spacepi::spacepictl;

SSHGen SSHGen::instance;

SSHGen::SSHGen() noexcept : Verb("ssh-gen", "Generates missing SSH keys for the system", true), success(true) {
}

bool SSHGen::run(const vector<string> &args) {
    struct statfs stat;
    if (statfs("/", &stat) < 0) {
        log(LogLevel::Warning) << "Unable to stat /: " << strerror(errno);
    } else if (unshare(CLONE_NEWNS) < 0) {
        log(LogLevel::Warning) << "Unable to create new mount namespace: " << strerror(errno);
    } else if (mount(nullptr, "/", nullptr, MS_PRIVATE, nullptr) < 0) {
        log(LogLevel::Warning) << "Unable to clone / mount into namespace: " << strerror(errno);
    } else if (mount(nullptr, "/", nullptr, MS_REMOUNT | (stat.f_flags & ~MS_RDONLY), nullptr) < 0) {
        log(LogLevel::Warning) << "Unable to remount / as read-write: " << strerror(errno);
    }

    pwd = getpwnam("spacepi");

    generate("/etc/ssh/id_rsa",             "rsa",   "SpacePi"     );
    generate("/etc/ssh/ssh_host_rsa_key",   "rsa",   "SpacePi-host");
    generate("/etc/ssh/ssh_host_ecdsa_key", "ecdsa", "SpacePi-host");
    return success;
}

void SSHGen::generate(const string &privateKey, const string &type, const string &comment) noexcept {
    string publicKey = privateKey + ".pub";
    if (!exists(privateKey) || !exists(publicKey)) {
        log(LogLevel::Info) << "Generating " << privateKey << "...";
        child proc("/usr/bin/ssh-keygen", "-q", "-t", type, "-N", "", "-C", comment, "-f", privateKey);
        proc.wait();
        if (proc.exit_code() != 0) {
            log(LogLevel::Warning) << "Unable to generate key: " << privateKey;
            success = false;
        }
        if (chown(privateKey.c_str(), pwd->pw_uid, pwd->pw_gid) < 0) {
            log(LogLevel::Warning) << "Unable to change owner of " << privateKey << ": " << strerror(errno);
        }
        if (chown(publicKey.c_str(), pwd->pw_uid, pwd->pw_gid) < 0) {
            log(LogLevel::Warning) << "Unable to change owner of " << publicKey << ": " << strerror(errno);
        }
        if (chmod(privateKey.c_str(), 0640) < 0) {
            log(LogLevel::Warning) << "Unable to change permissions of " << privateKey << ": " << strerror(errno);
        }
        if (chmod(publicKey.c_str(), 0644) < 0) {
            log(LogLevel::Warning) << "Unable to change permissions of " << publicKey << ": " << strerror(errno);
        }
    }
}

ostream &SSHGen::printHelp(ostream &os) const noexcept {
    return os << "\n";
}
