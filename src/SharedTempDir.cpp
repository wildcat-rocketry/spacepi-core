#include <memory>
#include <string>
#include <spacepi/liblinux/UniqueTempDir.hpp>
#include <spacepi/liblinux/SharedTempDir.hpp>
#include <utility>
#include <memory>

using namespace spacepi::liblinux;

SharedTempDir::SharedTempDir(const std::string &name){
    unique.reset(new UniqueTempDir(name));
}

SharedTempDir::SharedTempDir(UniqueTempDir &&dir){
    unique.reset(new UniqueTempDir(std::move(dir)));
}

const std::string &SharedTempDir::getPath() const noexcept{
    unique->getPath();
}

bool SharedTempDir::doesExist() const noexcept{
    return unique->doesExist();
}

void SharedTempDir::mkdir(){
    unique->mkdir();
}

void SharedTempDir::rmdir(){
    unique->rmdir();
}
                