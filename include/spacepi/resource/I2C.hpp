#ifndef SPACEPI_CORE_RESOURCE_I2C_HPP
#define SPACEPI_CORE_RESOURCE_I2C_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <spacepi/resource/BusTransaction.hpp>

namespace spacepi {
    namespace resource {
        /**
         * \brief A hardware resource which represents an I2C bus
         */
        class I2C : public Bus {
            public:
                /**
                 * \brief Get a pointer to an I2C object by its resource name
                 * 
                 * \param[in] name The resource name for which to get the I2C object
                 * 
                 * \return The I2C object, or a null pointer if the name is not registered
                 */
                static std::shared_ptr<I2C> get(const std::string &name);

                /**
                 * \brief Gets the BAUD rate of the bus
                 * 
                 * \return The BAUD rate
                 */
                virtual int getSpeed() const noexcept = 0;

                /**
                 * \brief Sets the BAUD rate of the bus
                 * 
                 * \param[in] speed The new BAUD rate
                 */
                virtual void setSpeed(int speed) = 0;

                /**
                 * \brief Determines if error checking is enabled for I2C running in SMBus mode
                 * 
                 * \return If it is enabled
                 */
                virtual bool isSMBusErrorCheckingEnabled() const noexcept = 0;

                /**
                 * \brief Enables SMBus error checking for this I2C bus
                 */
                virtual void enableSMBusErrorChecking() = 0;

                /**
                 * \brief Disabled SMBus error checking for this I2C bus
                 */
                virtual void disableSMBusErrorChecking() = 0;

                /**
                 * \brief Performs a one-bit write operation on the SMBus
                 * 
                 * \param[in] val The bit to write
                 */
                virtual void quickWriteSMBus(bool val) = 0;

                /**
                 * \brief Performs a one-byte read operation on the SMBus
                 * 
                 * \return The read byte
                 */
                virtual uint8_t readByteSMBus() = 0;

                /**
                 * \brief Performs a one-byte write operation on the SMBus
                 * 
                 * \param[in] data The byte to write
                 */
                virtual void writeByteSMBus(uint8_t data) = 0;

                /**
                 * \brief Performs a one-byte read command on the SMBus
                 * 
                 * \param[in] command The command ID
                 * 
                 * \return The read byte
                 */
                virtual uint8_t readByteSMBus(uint8_t command) = 0;

                /**
                 * \brief Performs a one-byte write command on the SMBus
                 * 
                 * \param[in] command The command ID
                 * \param[in] data The byte to write
                 */
                virtual void writeByteSMBus(uint8_t command, uint8_t data) = 0;

                /**
                 * \brief Performs a two-byte read command on the SMBus
                 * 
                 * \param[in] command The command ID
                 * 
                 * \return The read bytes
                 */
                virtual uint16_t readWordSMBus(uint8_t command) = 0;

                /**
                 * \brief Performs a two-byte write command on the SMBus
                 * 
                 * \param[in] command The command ID
                 * \param[in] data The bytes to write
                 */
                virtual void writeWordSMBus(uint8_t command, uint16_t data) = 0;

                /**
                 * \brief Performs a process call on the SMBus
                 * 
                 * \param[in] command The command ID
                 * \param[in] data The argument to the process
                 * 
                 * \return The return value of the process
                 */
                virtual uint16_t processCallSMBus(uint8_t command, uint16_t data) = 0;

                /**
                 * \brief Reads a block of data from the SMBus
                 * 
                 * \param[in] command The command ID
                 * \param[out] data The read data
                 */
                virtual void readBlockSMBus(uint8_t command, uint8_t *data) = 0;

                /**
                 * \brief Writes a block of data to the SMBus
                 * 
                 * \param[in] command The command ID
                 * \param[in] data The data to write
                 * \param[in] length The number of bytes to write
                 */
                virtual void writeBlockSMBus(uint8_t command, uint8_t *data, uint8_t length) = 0;

            protected:
                virtual void doTransaction(const std::vector<std::pair<uint8_t *, int16_t>> &steps) = 0;
        };
    }
}

#endif
