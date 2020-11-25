#include <array>
#include <chrono>
#include <memory>
#include <string>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/resource/Processor.hpp>
#include <spacepi/resource/ResourceFactory.hpp>

namespace spacepi {
    namespace resource {
        namespace detail {
            class MockProcessor : public Processor, private spacepi::log::AutoLog<decltype("core:resource:mock:processor"_autolog)> {
                public:
                    MockProcessor(const std::string &name) noexcept;

                    std::chrono::seconds getUptime();
                    std::array<double, 3> getLoadAverage();
                    ProcessInfo getProcessInfo();

                private:
                    std::string name;
            };

            class MockProcessorFactory : public ResourceFactory<Processor> {
                public:
                    static MockProcessorFactory instance;

                    MockProcessorFactory();

                private:
                    std::shared_ptr<Processor> create(const std::string &name);
            };
        }
    }
}

using namespace std;
using namespace std::chrono;
using namespace spacepi::log;
using namespace spacepi::resource;
using namespace spacepi::resource::detail;

MockProcessorFactory MockProcessorFactory::instance;

MockProcessor::MockProcessor(const string &name) noexcept : name(name) {
    log(LogLevel::Debug) << "Created new Processor '" << name << "'.";
}

seconds MockProcessor::getUptime() {
    log(LogLevel::Debug) << "Getting uptime info from Processor '" << name << "'.";
    return seconds(42);
}

array<double, 3> MockProcessor::getLoadAverage() {
    log(LogLevel::Debug) << "Getting load info from Processor '" << name << "'.";
    array<double, 3> arr;
    arr[0] = 0.5;
    arr[1] = 0.3;
    arr[2] = 0.2;
    return arr;
}

ProcessInfo MockProcessor::getProcessInfo() {
    log(LogLevel::Debug) << "Getting process info from Processor '" << name << "'.";
    return ProcessInfo(1000, 900, 10, 5);
}

MockProcessorFactory::MockProcessorFactory() : ResourceFactory<Processor>("mock") {
}

shared_ptr<Processor> MockProcessorFactory::create(const string &name) {
    return shared_ptr<Processor>(new MockProcessor(name));
}

shared_ptr<Processor> Processor::get(const string &name) {
    return ResourceFactory<Processor>::get(name);
}

ProcessInfo::ProcessInfo(int numRunning, int numSleeping, int numStopped, int numZombied) noexcept : numRunning(numRunning), numSleeping(numSleeping), numStopped(numStopped), numZombied(numZombied) {
}

int ProcessInfo::countRunning() const noexcept {
    return numRunning;
}

int ProcessInfo::countSleeping() const noexcept {
    return numSleeping;
}

int ProcessInfo::countStopped() const noexcept {
    return numStopped;
}

int ProcessInfo::countZombied() const noexcept {
    return numZombied;
}
