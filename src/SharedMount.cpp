#include <exception>
#include <memory>
#include <spacepi/liblinux/SharedMount.hpp>
#include <spacepi/liblinux/UniqueMount.hpp>
#include <string>
#include <utility>
#include <vector>

using namespace std;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::detail;

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

vector<SharedMount>::vector(const vector<SharedMount> &copy) noexcept : vector<SharedMount, SharedMountAllocator>(copy, SharedMountAllocator()) {
}

vector<SharedMount>::vector(vector<SharedMount> &&move) noexcept : vector<SharedMount, SharedMountAllocator>(std::move(move), SharedMountAllocator()) {
}

vector<SharedMount>::~vector() noexcept(false) {
    reverse_iterator it;
    try {
        for (it = rbegin(); it != rend(); ++it) {
            it->forceUnmount();
        }
    } catch (const exception &) {
        for (; it != rend(); ++it) {
            try {
                it->forceUnmount();
            } catch (const exception &) {
            }
        }
        throw;
    }
}
