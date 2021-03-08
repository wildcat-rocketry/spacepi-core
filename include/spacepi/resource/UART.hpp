#ifndef SPACEPI_CORE_RESOURCE_UART_HPP
#define SPACEPI_CORE_RESOURCE_UART_HPP

#include <istream>
#include <memory>
#include <streambuf>
#include <string>

namespace spacepi {
    namespace resource {
        /**
         * \brief A hardware resource which represents a serial UART bus
         */
        class UART : public std::iostream, public std::streambuf {
            public:
                /**
                 * \brief Initialize the UART
                 */
                UART();

                /**
                 * \brief Copy-construct the UART
                 * 
                 * \param[in] copy The original object
                 */
                UART(const UART &copy);

                /**
                 * \brief Destroys this UART resource
                 */
                virtual ~UART() = default;

                /**
                 * \brief Copy-assign the UART
                 * 
                 * \param[in] copy The original object
                 * 
                 * \return \c this
                 */
                UART &operator =(const UART &copy);

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

            protected:
                /**
                 * \brief Read some data from the UART bus.
                 * 
                 * \param[out] buffer The data which was read
                 * \param[in] count The maximum amount of data to read
                 * 
                 * \return The actual amount of data which was read
                 */
                virtual std::streamsize xsgetn(char *buffer, std::streamsize count) = 0;

                /**
                 * \brief Write some data to the UART bus
                 * 
                 * \param[in] buffer The data to write
                 * \param[in] count The maximum amount of data to write
                 * 
                 * \return The actual amount of data which was written
                 */
                virtual std::streamsize xsputn(char *buffer, std::streamsize count) = 0;

                /**
                 * \brief Peek the next character to read but don't remove
                 * 
                 * \return The next character to read or EOL
                 */
                virtual int underflow();

                /**
                 * \brief Take the next character off the UART
                 * 
                 * \return The next character on the UART or EOL
                 */
                virtual int uflow();

                /**
                 * \brief Character taken off the UART but not processed.
                 */
                char last = 0;
        };
    }
}

#endif
