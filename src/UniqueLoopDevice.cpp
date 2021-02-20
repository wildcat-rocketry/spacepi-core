#include <exception>
#include <string>
#include <spacepi/liblinux/Config.hpp>
#include <spacepi/liblinux/UniqueLoopDevice.hpp>
#include <spacepi/liblinux/UniqueProcess.hpp>
#include <SpacePi.hpp>

using namespace spacepi::liblinux;
using namespace spacepi::util;

UniqueLoopDevice::UniqueLoopDevice(const std::string &imageFile){
    this->imageFile = imageFile;
    mount();
    mounted = true;
}

UniqueLoopDevice::~UniqueLoopDevice() noexcept(false) {
    unmount();
}

UniqueLoopDevice::UniqueLoopDevice(UniqueLoopDevice &&move) noexcept{
    this->imageFile = move.imageFile;
    this->baseDevice = move.baseDevice;
    
    this->mounted = move.mounted;
    move.mounted = false;
}

UniqueLoopDevice &UniqueLoopDevice::operator =(UniqueLoopDevice &&move) noexcept{
    this->imageFile = move.imageFile;
    this->baseDevice = move.baseDevice;

    this->mounted = move.mounted;
    move.mounted = false;
    return *this;
}

std::string UniqueLoopDevice::getBlockDevice(int partNo) const{
    return baseDevice + std::to_string(partNo + 1);
}

bool UniqueLoopDevice::isMounted() const noexcept{
    return mounted;
}

void UniqueLoopDevice::mount(){
    if (!mounted) {
        UniqueProcess process(false,true,false,KPARTX_EXECUTABLE,{ "-asv",imageFile });
        while (!process.output().eof()) {
            std::string word;
            process.output() >> word;
            if (word.size() > 4 && word.substr(0, 4) == "loop") {
                baseDevice = "/dev/mapper/" + word.substr(0, word.size() - 1);
                process.wait();
                if (process.getExitCode() != 0) {
                    throw EXCEPTION(ResourceException("Error mounting loop device."));
                }
                mounted = true;
                return;
            }
        }
        throw EXCEPTION(ResourceException("Error mounting loop device."));
    }
}

void UniqueLoopDevice::unmount(){
    if (mounted) {
        UniqueProcess process(false,false,false,KPARTX_EXECUTABLE,{ "-d",imageFile });
        process.wait();
        if (process.getExitCode() != 0) {
            throw EXCEPTION(ResourceException("Error unmounting loop device."));
        }
        mounted = false;
    }
}

void UniqueLoopDevice::forceUnmount() {
    try {
        unmount();
    } catch (const std::exception &) {
        mounted = false;
        throw;
    }
}
