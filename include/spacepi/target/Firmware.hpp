#ifndef SPACEPI_TARGET_RPI_FIRMWARE_HPP
#define SPACEPI_TARGET_RPI_FIRMWARE_HPP

#include <string>

namespace spacepi {
    namespace target {
        class Firmware {
            public:
                Firmware();

                const std::string &getDownloadRepo() const noexcept;

            private:
                std::string downloadRepo;
        };
    }
}

#endif
