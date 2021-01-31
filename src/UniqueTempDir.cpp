#include <algorithm>
#include <boost/filesystem.hpp>
#include <ctime>
#include <functional>
#include <iostream>
#include <spacepi/liblinux/SystemCaller.hpp>
#include <spacepi/liblinux/UniqueTempDir.hpp>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

using namespace spacepi::liblinux;
using namespace boost::filesystem;

UniqueTempDir::UniqueTempDir(const std::string &name){
    this->name = name;
    exists = false;
    mkdir();
}

UniqueTempDir::~UniqueTempDir(){
    rmdir();
}

UniqueTempDir::UniqueTempDir(UniqueTempDir &&move) noexcept{
    this->name = move.name;
    this->path = move.path;
    this->exists = move.exists;
    move.exists = false;
}

UniqueTempDir &UniqueTempDir::operator =(UniqueTempDir &&move) noexcept{
    this->name = move.name;
    this->path = move.path;
    this->exists = move.exists;
    move.exists = false;
    return *this;
}

const std::string &UniqueTempDir::getPath() const noexcept{
    return path;
}

bool UniqueTempDir::doesExist() const noexcept{
    struct stat st;
    handle(::stat(path.c_str(),&st)) << "Directory does not exist: " << path << ":" << SyscallErrorString;
    return exists;
}

void UniqueTempDir::mkdir(){
    if (!exists) {
        while (true) {
            makePath(name);
            if (access(path.c_str(), F_OK) < 0) {
                handle(::mkdir(path.c_str(),0755)) << "Failed to make directory: " << path << ":" << SyscallErrorString;
                exists = true;
                return;
            }
        }
    }
}

void UniqueTempDir::rmdir(){
    if (exists) {
        remove_all(path);
        exists = false;
    }
}

void UniqueTempDir::makePath(const std::string &name){
    std::string pathending = randomString(8);
    path = "/tmp/spacepi-" + name + "-" + pathending;
}

std::string UniqueTempDir::randomString(size_t length)
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}
