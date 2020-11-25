#ifndef SPACEPI_CORE_RESOURCE_SPI_HPP
#define SPACEPI_CORE_RESOURCE_SPI_HPP

#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
#include <spacepi/resource/BusTransaction.hpp>

namespace spacepi {
    namespace resource {
        class SPI : public Bus {
            public:
                enum Mode {
                    ClockFallingEdge = 0,
                    ClockRisingEdge = 1,
                    ClockActiveHigh = 0,
                    ClockActiveLow = 2,

                    Mode0 = 0,
                    Mode1 = 1,
                    Mode2 = 2,
                    Mode3 = 3,

                    MSBFirst = 0,
                    LSBFirst = 4
                };

                static std::shared_ptr<SPI> get(const std::string &name);

                virtual enum Mode getMode() const noexcept = 0;
                virtual void setMode(enum Mode mode) = 0;

                virtual int getSpeed() const noexcept = 0;
                virtual void setSpeed(int speed) = 0;

            protected:
                virtual void doTransaction(const std::vector<std::pair<uint8_t *, int16_t>> &steps) = 0;
        };
    }
}

std::ostream &operator <<(std::ostream &os, enum spacepi::resource::SPI::Mode mode);

#endif
