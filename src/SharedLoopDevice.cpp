#include <memory>
#include <string>
#include <spacepi/liblinux/UniqueLoopDevice.hpp>
#include <spacepi/liblinux/SharedLoopDevice.hpp>

using namespace spacepi::liblinux;

SharedLoopDevice::SharedLoopDevice(const std::string &imageFile){
    unique.reset(new UniqueLoopDevice(imageFile));
}

SharedLoopDevice::SharedLoopDevice(UniqueLoopDevice &&loop){
    unique.reset(new UniqueLoopDevice(std::move(loop)));
}

SharedLoopDevice::operator bool() const noexcept {
    return (bool) unique;
}

std::string SharedLoopDevice::getBlockDevice(int partNo) const{
    return unique->getBlockDevice(partNo);
}

bool SharedLoopDevice::isMounted() const noexcept{
    bool mounted = unique->isMounted();
    return mounted;
}

void SharedLoopDevice::mount(){
    unique->mount();
}

void SharedLoopDevice::unmount(){
    unique->unmount();
}