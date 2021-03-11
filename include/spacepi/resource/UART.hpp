#ifndef SPACEPI_CORE_RESOURCE_UART_HPP
#define SPACEPI_CORE_RESOURCE_UART_HPP

#include <memory>
#include <string>
#include <spacepi/resource/Stream.hpp>

namespace spacepi {
    namespace resource {
        /**
         * \brief A hardware resource which represents a serial UART bus
         */
        class UART : public Stream {
            public:
                /**
                 * \brief Get a pointer to a UART object by its resource name
                 * 
                 * \param[in] name The resource name for which to get the UART object
                 * 
                 * \return The UART object, or a null pointer if the name is not registered
                 */
                static std::shared_ptr<UART> get(const std::string &name);

                /**
                 * \brief Gets the BAUD rate of the bus
                 * 
                 * \return The BAUD rate
                 */
                virtual int getBAUDRate() const noexcept = 0;

                /**
                 * \brief Sets the BAUD rate of the bus
                 * 
                 * \param[in] baud The new BAUD rate
                 */
                virtual void setBAUDRate(int baud) = 0;
        };
    }
}

#endif
