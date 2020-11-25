#ifndef SPACEPI_CORE_RESOURCE_PROCESSOR_HPP
#define SPACEPI_CORE_RESOURCE_PROCESSOR_HPP

#include <array>
#include <chrono>
#include <memory>
#include <string>

namespace spacepi {
    namespace resource {
        class ProcessInfo {
            public:
                ProcessInfo(int numRunning, int numSleeping, int numStopped, int numZombied) noexcept;

                int countRunning() const noexcept;
                int countSleeping() const noexcept;
                int countStopped() const noexcept;
                int countZombied() const noexcept;

            private:
                int numRunning;
                int numSleeping;
                int numStopped;
                int numZombied;
        };

        class Processor {
            public:
                static std::shared_ptr<Processor> get(const std::string &name);

                virtual std::chrono::seconds getUptime() = 0;
                virtual std::array<double, 3> getLoadAverage() = 0;
                virtual ProcessInfo getProcessInfo() = 0;
        };
    }
}

#endif
