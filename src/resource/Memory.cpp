#include <cstdint>
#include <memory>
#include <string>
#include <spacepi/resource/Memory.hpp>
#include <spacepi/resource/ResourceFactory.hpp>

using namespace std;
using namespace spacepi::resource;

shared_ptr<Memory> Memory::get(const string &name) {
    return ResourceFactory<Memory>::get(name);
}

MemoryInfo::MemoryInfo(uint64_t free, uint64_t total) noexcept : free(free), total(total) {
}

uint64_t MemoryInfo::bytesUsed() const noexcept {
    return total - free;
}

uint64_t MemoryInfo::bytesFree() const noexcept {
    return free;
}

uint64_t MemoryInfo::bytesTotal() const noexcept {
    return total;
}

RAMInfo::RAMInfo(const MemoryInfo &real, const MemoryInfo &swap) noexcept : real(real), swap(swap) {
}

const MemoryInfo &RAMInfo::getReal() const noexcept {
    return real;
}

const MemoryInfo &RAMInfo::getSwap() const noexcept {
    return swap;
}
