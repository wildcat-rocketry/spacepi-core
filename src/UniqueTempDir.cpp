#include <string>
#include <spacepi/liblinux/UniqueTempDir.hpp>

using namespace spacepi::liblinux;

UniqueTempDir::UniqueTempDir(const std::string &name){

}

UniqueTempDir::~UniqueTempDir(){

}

UniqueTempDir::UniqueTempDir(UniqueTempDir &&move) noexcept{

}

UniqueTempDir &UniqueTempDir::operator =(UniqueTempDir &&move) noexcept{

}

const std::string &UniqueTempDir::getPath() const noexcept{

}

bool UniqueTempDir::doesExist() const noexcept{

}

void UniqueTempDir::mkdir(){

}

void UniqueTempDir::rmdir(){

}
