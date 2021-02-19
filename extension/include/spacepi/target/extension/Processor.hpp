#ifndef SPACEPI_TARGETLIB_LINUX_EXTENSION_PROCESSOR_HPP
#define SPACEPI_TARGETLIB_LINUX_EXTENSION_PROCESSOR_HPP

#include <array>
#include <chrono>
#include <SpacePi.hpp>

namespace spacepi {
    namespace target {
        namespace extension {
            class Processor : public spacepi::resource::Processor, private spacepi::log::AutoLog<decltype("linux:processor"_autolog)> {
                public:
                    std::chrono::seconds getUptime();
                    std::array<double, 3> getLoadAverage();
                    spacepi::resource::ProcessInfo getProcessInfo();
            };
        }
    }
}

#endif
