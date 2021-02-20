#include <memory>
#include <spacepi/liblinux/SharedMount.hpp>
#include <spacepi/liblinux/UniqueMount.hpp>
#include <string>
#include <utility>

using namespace spacepi::liblinux;

SharedMount::SharedMount(const std::string &blockDevice, const std::string &mountPoint, const std::string &options, const std::string &type){
    unique.reset(new UniqueMount(blockDevice,mountPoint,options,type));
}

SharedMount::SharedMount(UniqueMount &&mount){
    unique.reset(new UniqueMount(std::move(mount)));
}

SharedMount::~SharedMount() noexcept(false) {
    if (unique && unique.unique()) {
        unique->forceUnmount();
    }
}

bool SharedMount::isMounted() const noexcept{
    bool mounted = unique->isMounted();
    return mounted;
}

void SharedMount::mount(){
    unique->mount();
}

void SharedMount::unmount(){
    unique->unmount();
}

void SharedMount::forceUnmount() {
    unique->forceUnmount();
}

void SharedMount::remount(const std::string &options) {
    unique->remount(options);
}
