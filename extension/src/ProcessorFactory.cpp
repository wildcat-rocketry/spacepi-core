#include <memory>
#include <string>
#include <SpacePi.hpp>
#include <spacepi/target/extension/Processor.hpp>
#include <spacepi/target/extension/ProcessorFactory.hpp>

using namespace std;
using namespace spacepi;
using namespace spacepi::resource;
using namespace spacepi::target;
using namespace spacepi::target::extension;

ProcessorFactory ProcessorFactory::instance;

ProcessorFactory::ProcessorFactory() noexcept : ResourceFactory<resource::Processor>("procfs") {
}

shared_ptr<resource::Processor> ProcessorFactory::create(const string &name) noexcept {
    shared_ptr<resource::Processor> mem = ptr.lock();
    if (!mem) {
        mem.reset(new extension::Processor());
        ptr = mem;
    }
    return mem;
}
