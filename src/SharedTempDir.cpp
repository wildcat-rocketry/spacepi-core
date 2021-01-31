#include <memory>
#include <string>
#include <spacepi/liblinux/UniqueTempDir.hpp>
#include <spacepi/liblinux/SharedTempDir.hpp>
#include <utility>
#include <memory>

using namespace spacepi::liblinux;

SharedTempDir::SharedTempDir(const std::string &name){
    //unique.reset(new SharedTempDir(std::move(name)));
}

SharedTempDir::SharedTempDir(UniqueTempDir &&dir){
    //unique.reset(new SharedTempDir(std::move(dir)));
}

SharedTempDir::~SharedTempDir(){

}

SharedTempDir::SharedTempDir(const SharedTempDir &copy) noexcept{
    //unique.reset(new UniqueTempDir(std::move(copy)));
}

SharedTempDir &SharedTempDir::operator =(const SharedTempDir &copy) noexcept{
   // unique.reset(new UniqueTempDir(std::move(copy)));
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
                