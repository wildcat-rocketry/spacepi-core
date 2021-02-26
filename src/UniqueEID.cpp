#include <exception>
#include <sys/types.h>
#include <unistd.h>
#include <spacepi/liblinux/SystemCaller.hpp>
#include <spacepi/liblinux/UniqueEID.hpp>

using namespace std;
using namespace spacepi::liblinux;

UniqueEID::UniqueEID(uid_t uid, gid_t gid) : oldUid(getuid()), oldGid(getgid()), newUid(uid), newGid(gid), active(false) {
    setId();
}

UniqueEID::~UniqueEID() noexcept(false) {
    unsetId();
}

UniqueEID::UniqueEID(UniqueEID &&move) noexcept : oldUid(move.oldUid), oldGid(move.oldGid), newUid(move.newUid), newGid(move.newGid), active(move.active) {
    move.active = false;
}

UniqueEID &UniqueEID::operator =(UniqueEID &&move) noexcept {
    oldUid = move.oldUid;
    oldGid = move.oldGid;
    newUid = move.newUid;
    newGid = move.newGid;
    active = move.active;
    move.active = false;
    return *this;
}

bool UniqueEID::isActive() const noexcept {
    return active;
}

void UniqueEID::setId() {
    if (!active) {
        handle(setregid(-1, newGid))
            << "Failed to set EGID: " << SyscallErrorString;
        try {
            handle(setreuid(-1, newUid))
                << "Failed to set EUID: " << SyscallErrorString;
        } catch (const exception &) {
            setegid(oldGid);
            throw;
        }
        active = true;
    }
}

void UniqueEID::unsetId() {
    if (active) {
        handle(seteuid(oldUid))
            << "Failed to reset EUID: " << SyscallErrorString;
        handle(setegid(oldGid))
            << "Failed to reset EGID: " << SyscallErrorString;
        active = false;
    }
}

void UniqueEID::forceUnsetId() {
    try {
        unsetId();
    } catch (const exception &) {
        active = false;
        throw;
    }
}
