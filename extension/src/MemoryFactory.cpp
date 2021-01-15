#include <memory>
#include <string>
#include <SpacePi.hpp>
#include <spacepi/target/extension/Memory.hpp>
#include <spacepi/target/extension/MemoryFactory.hpp>

using namespace std;
using namespace spacepi;
using namespace spacepi::resource;
using namespace spacepi::target;
using namespace spacepi::target::extension;

MemoryFactory MemoryFactory::instance;

MemoryFactory::MemoryFactory() noexcept : ResourceFactory<resource::Memory>("procfs") {
}

shared_ptr<resource::Memory> MemoryFactory::create(const string &name) noexcept {
    shared_ptr<resource::Memory> mem = ptr.lock();
    if (!mem) {
        mem.reset(new extension::Memory());
        ptr = mem;
    }
    return mem;
}
