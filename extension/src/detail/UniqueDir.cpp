#include <dirent.h>
#include <spacepi/target/extension/detail/UniqueDir.hpp>

using namespace spacepi::target::extension::detail;

UniqueDir::UniqueDir(DIR *dir) noexcept : dir(dir) {
}

UniqueDir::~UniqueDir() noexcept {
    if (*this) {
        reset();
    }
}

UniqueDir::UniqueDir(UniqueDir &&move) noexcept {
    reset(move.dir);
    move.release();
}

UniqueDir &UniqueDir::operator =(UniqueDir &&move) noexcept {
    reset(move.dir);
    move.release();
    return *this;
}

UniqueDir &UniqueDir::operator =(DIR *dir) noexcept {
    reset(dir);
    return *this;
}

UniqueDir::operator DIR *() const noexcept {
    return dir;
}

UniqueDir::operator bool() const noexcept {
    return dir;
}

void UniqueDir::release() noexcept {
    dir = nullptr;
}

void UniqueDir::reset(DIR *dir) noexcept {
    if (*this) {
        closedir(this->dir);
    }
    this->dir = dir;
}
