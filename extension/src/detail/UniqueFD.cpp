#include <unistd.h>
#include <spacepi/target/extension/detail/UniqueFD.hpp>

using namespace spacepi::target::extension::detail;

UniqueFD::UniqueFD(int fd) noexcept : fd(fd) {
}

UniqueFD::~UniqueFD() noexcept {
    if (*this) {
        reset();
    }
}

UniqueFD::UniqueFD(UniqueFD &&move) noexcept {
    reset(move.fd);
    move.release();
}

UniqueFD &UniqueFD::operator =(UniqueFD &&move) noexcept {
    reset(move.fd);
    move.release();
    return *this;
}

UniqueFD &UniqueFD::operator =(int fd) noexcept {
    reset(fd);
    return *this;
}

UniqueFD::operator int() const noexcept {
    return fd;
}

UniqueFD::operator bool() const noexcept {
    return fd >= 0;
}

void UniqueFD::release() noexcept {
    fd = -1;
}

void UniqueFD::reset(int fd) noexcept {
    if (*this) {
        close(this->fd);
    }
    this->fd = fd;
}
