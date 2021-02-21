#include <exception>
#include <string>
#include <utility>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <spacepi/liblinux/SystemCaller.hpp>
#include <spacepi/liblinux/UniqueChroot.hpp>

using namespace std;
using namespace spacepi::liblinux;

UniqueChroot::UniqueChroot(const string &root) : root(root), realRoot(open("/", O_DIRECTORY)), oldCwd(open(".", O_DIRECTORY)), active(false) {
    try {
        handle(realRoot) << "Failed to get reference to real root directory: " << SyscallErrorString;
        handle(oldCwd) << "Failed to get reference to current directory: " << SyscallErrorString;
    } catch (const exception &) {
        if (realRoot > 0) {
            close(realRoot);
        }
        if (oldCwd > 0) {
            close(oldCwd);
        }
        throw;
    }
    enter();
}

UniqueChroot::~UniqueChroot() noexcept(false) {
    escape();
    if (realRoot > 0) {
        close(realRoot);
        realRoot = -1;
    }
    if (oldCwd > 0) {
        close(oldCwd);
        oldCwd = -1;
    }
}

UniqueChroot::UniqueChroot(UniqueChroot &&move) noexcept : root(std::move(move.root)), realRoot(move.realRoot), active(move.active) {
    move.realRoot = -1;
    move.active = false;
}

UniqueChroot &UniqueChroot::operator =(UniqueChroot &&move) noexcept {
    new (this) UniqueChroot(std::move(move));
    return *this;
}

bool UniqueChroot::isActive() const noexcept {
    return active;
}

void UniqueChroot::enter() {
    if (!active) {
        handle(chroot(root.c_str())) << "Failed to chroot: " << SyscallErrorString;
        active = true;
        handle(chdir("/")) << "Failed to chdir into chroot: " << SyscallErrorString;
    }
}

void UniqueChroot::escape() {
    if (active) {
        handle(fchdir(realRoot)) << "Failed to chdir out of chroot: " << SyscallErrorString;
        handle(chroot(".")) << "Failed to exit chroot: " << SyscallErrorString;
        active = false;
        handle(fchdir(oldCwd)) << "Failed to chdir back to cwd: " << SyscallErrorString;
    }
}

void UniqueChroot::forceEscape() {
    try {
        escape();
    } catch (const std::exception &) {
        active = false;
        throw;
    }
}
