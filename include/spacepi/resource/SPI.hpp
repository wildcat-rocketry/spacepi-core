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
        /**
         * \brief A hardware resource which represents a SPI bus
         */
        class SPI : public Bus {
            public:
                /**
                 * \brief Enumeration representing different modes in which a SPI bus can operate
                 */
                enum Mode {
                    /**
                     * \brief The data latches on the falling edge of the clock signal
                     */
                    ClockFallingEdge = 0,
                    /**
                     * \brief The data latches on the rising edge of the clock signal
                     */
                    ClockRisingEdge = 1,
                    /**
                     * \brief The clock signal is active-high
                     */
                    ClockActiveHigh = 0,
                    /**
                     * \brief The clock signal is active-low
                     */
                    ClockActiveLow = 2,

                    /**
                     * \brief Standard mode 0
                     */
                    Mode0 = 0,

                    /**
                     * \brief Standard mode 1
                     */
                    Mode1 = 1,

                    /**
                     * \brief Standard mode 2
                     */
                    Mode2 = 2,

                    /**
                     * \brief Standard mode 3
                     */
                    Mode3 = 3,

                    /**
                     * \brief The most-significant bit is the first bit to be transmitted
                     */
                    MSBFirst = 0,

                    /**
                     * \brief The least-significant bit is the first bit to be transmitted
                     */
                    LSBFirst = 4
                };

                /**
                 * \brief Get a pointer to a SPI object by its resource name
                 * 
                 * \param[in] name The resource name for which to get the SPI object
                 * 
                 * \return The SPI object, or a null pointer if the name is not registered
                 */
                static std::shared_ptr<SPI> get(const std::string &name);

                /**
                 * Gets the current mode in which the bus is operating
                 * 
                 * \return The mode
                 */
                virtual enum Mode getMode() const noexcept = 0;

                /**
                 * \brief Change the current mode in which the bus is operating
                 * 
                 * \param[in] mode The mode
                 */
                virtual void setMode(enum Mode mode) = 0;

                /**
                 * \brief Get the BAUD rate of the bus
                 * 
                 * \return The BAUD rate
                 */
                virtual int getSpeed() const noexcept = 0;

                /**
                 * \brief Set the BAUD rate of the bus
                 * 
                 * \param[in] speed The new BAUD rate
                 */
                virtual void setSpeed(int speed) = 0;

            protected:
                virtual void doTransaction(const std::vector<std::pair<uint8_t *, int16_t>> &steps) = 0;
        };
    }
}

/**
 * \brief Writes the logical name of a spacepi::resource::SPI::Mode enum constant to a stream
 * 
 * \param[in,out] os The stream to write to
 * \param[in] mode The enum constant
 * 
 * \return \c os
 */
std::ostream &operator <<(std::ostream &os, enum spacepi::resource::SPI::Mode mode);

#endif
