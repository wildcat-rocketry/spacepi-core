#ifndef SPACEPI_TARGET_RPI_EXTENSION_MEMORY_HPP
#define SPACEPI_TARGET_RPI_EXTENSION_MEMORY_HPP

#include <SpacePi.hpp>

namespace spacepi {
    namespace target {
        namespace extension {
            class Memory : public spacepi::resource::Memory {
                public:
                    spacepi::resource::MemoryInfo getDiskInfo();
                    spacepi::resource::RAMInfo getRAMInfo();
            };
        }
    }
}

#endif
