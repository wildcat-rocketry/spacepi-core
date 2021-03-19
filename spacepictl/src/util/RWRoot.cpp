#include <cerrno>
#include <cstring>
#include <string>
#include <sys/mount.h>
#include <sys/vfs.h>
#include <fstab.h>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/util/RWRoot.hpp>

using namespace std;
using namespace spacepi::log;
using namespace spacepi::util;
using namespace spacepi::spacepictl::util;

RWRoot::RWRoot() {
    if (statfs("/", &stat) < 0) {
        throw EXCEPTION(ResourceException(string("Unable to stat /: ") + strerror(errno)));
    }
    if (stat.f_flags & MS_RDONLY) {
        if (mount(nullptr, "/", nullptr, MS_REMOUNT | (stat.f_flags & ~MS_RDONLY), nullptr) < 0) {
            throw EXCEPTION(ResourceException(string("Unable to remount / as read-write: ") + strerror(errno)));
        }
    }
}

RWRoot::~RWRoot() noexcept {
    struct fstab *mnt = getfsfile("/");
    if (!mnt) {
        log(LogLevel::Warning) << "Unable to find entry in fstab for /";
        return;
    }
    if (!strcmp(FSTAB_RO, mnt->fs_type)) {
        if (mount(nullptr, "/", nullptr, MS_REMOUNT | stat.f_flags | MS_RDONLY, nullptr) < 0) {
            log(LogLevel::Warning) << "Unable to remount / as read-only: " << strerror(errno);
        }
    }
}
