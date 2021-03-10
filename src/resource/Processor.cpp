#include <memory>
#include <string>
#include <spacepi/resource/Processor.hpp>
#include <spacepi/resource/ResourceFactory.hpp>

using namespace std;
using namespace std::chrono;
using namespace spacepi::resource;

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
