#include <memory>
#include <string>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/resource/Memory.hpp>
#include <spacepi/resource/ResourceFactory.hpp>

namespace spacepi {
    namespace resource {
        namespace detail {
            class MockMemory : public Memory, private spacepi::log::AutoLog<decltype("core:resource:mock:memory"_autolog)> {
                public:
                    MockMemory(const std::string &name) noexcept;

                    MemoryInfo getDiskInfo();
                    RAMInfo getRAMInfo();

                private:
                    std::string name;
            };

            class MockMemoryFactory : public ResourceFactory<Memory> {
                public:
                    static MockMemoryFactory instance;

                    MockMemoryFactory();

                private:
                    std::shared_ptr<Memory> create(const std::string &name);
            };
        }
    }
}

using namespace std;
using namespace spacepi::log;
using namespace spacepi::resource;
using namespace spacepi::resource::detail;

MockMemoryFactory MockMemoryFactory::instance;

MockMemory::MockMemory(const string &name) noexcept : name(name) {
    log(LogLevel::Debug) << "Created new Memory '" << name << "'.";
}

MemoryInfo MockMemory::getDiskInfo() {
    log(LogLevel::Debug) << "Getting disk info from Memory '" << name << "'.";
    return MemoryInfo(4000000000, 16000000000);
}

RAMInfo MockMemory::getRAMInfo() {
    log(LogLevel::Debug) << "Getting RAM info from Memory '" << name << "'.";
    return RAMInfo(MemoryInfo(3000000000, 4000000000), MemoryInfo(6000000000, 8000000000));
}

MockMemoryFactory::MockMemoryFactory() : ResourceFactory<Memory>("mock") {
}

shared_ptr<Memory> MockMemoryFactory::create(const string &name) {
    return shared_ptr<Memory>(new MockMemory(name));
}
