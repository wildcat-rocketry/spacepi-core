#ifndef SPACEPI_TARGET_RPI_EXTENSION_SPI_HPP
#define SPACEPI_TARGET_RPI_EXTENSION_SPI_HPP

#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include <SpacePi.hpp>
#include <spacepi/target/extension/detail/UniqueFD.hpp>

namespace spacepi {
    namespace target {
        namespace extension {
            class SPI : public spacepi::resource::SPI {
                public:
                    SPI(const std::string &deviceName);

                    enum Mode getMode() const noexcept;
                    void setMode(enum Mode mode);

                    int getSpeed() const noexcept;
                    void setSpeed(int speed);

                protected:
                    void doTransaction(const std::vector<std::pair<uint8_t *, int16_t>> &steps);

                private:
                    void throwError(int returnCode);

                    detail::UniqueFD fd;
                    enum Mode mode;
                    int speed;
            };
        }
    }
}

#endif
