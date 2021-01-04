#ifndef SPACEPI_CORE_RESOURCE_BUSTRANSACTION_HPP
#define SPACEPI_CORE_RESOURCE_BUSTRANSACTION_HPP

#include <cstdint>
#include <utility>
#include <vector>

namespace spacepi {
    namespace resource {
        class BusTransaction;

        /**
         * \brief Base class for hardware resources which can send or recieve packets as a stream of data types
         */
        class Bus {
            friend class BusTransaction;

            public:
                /**
                 * \brief Transmit an 8-bit field to the end of the BusTransaction
                 * 
                 * \param[in] data The data to transmit
                 * 
                 * \return The BusTransaction to allow \c << chaining
                 */
                BusTransaction operator <<(uint8_t &data);

                /**
                 * \brief Transmit a 16-bit field to the end of the BusTransaction
                 * 
                 * \param[in] data The data to transmit
                 * 
                 * \return The BusTransaction to allow \c << chaining
                 */
                BusTransaction operator <<(uint16_t &data);

                /**
                 * \brief Transmit a 32-bit field to the end of the BusTransaction
                 * 
                 * \param[in] data The data to transmit
                 * 
                 * \return The BusTransaction to allow \c << chaining
                 */
                BusTransaction operator <<(uint32_t &data);

                /**
                 * \brief Receive an 8-bit field to the end of the BusTransaction
                 * 
                 * \param[out] data The data to receive
                 * 
                 * \return The BusTransaction to allow \c >> chaining
                 */
                BusTransaction operator >>(uint8_t &data);

                /**
                 * \brief Receive a 16-bit field to the end of the BusTransaction
                 * 
                 * \param[out] data The data to receive
                 * 
                 * \return The BusTransaction to allow \c >> chaining
                 */
                BusTransaction operator >>(uint16_t &data);

                /**
                 * \brief Receive a 32-bit field to the end of the BusTransaction
                 * 
                 * \param[out] data The data to receive
                 * 
                 * \return The BusTransaction to allow \c >> chaining
                 */
                BusTransaction operator >>(uint32_t &data);

                /**
                 * \brief Receive a buffered field to the end of the BusTransaction
                 * 
                 * \param[out] data The data to receive
                 * \param[in] length The number of bytes to receive
                 * 
                 * \return The BusTransaction to allow chaining
                 */
                BusTransaction read(uint8_t *data, uint16_t length);

                /**
                 * \brief Transmit a buffered field to the end of the BusTransaction
                 * 
                 * \param[in] data The data to transmit
                 * \param[in] length The number of bytes to transmit
                 * 
                 * \return The BusTransaction to allow chaining
                 */
                BusTransaction write(uint8_t *data, uint16_t length);

            private:
                /**
                 * \brief Performs an operation on the bus
                 * 
                 * Each element in the \c steps vector contains a pointer to some data and a length (in bytes).
                 * Positive lengths denote a receive operation, and negative lengths denote a transmit operation.
                 * 
                 * \param[in] steps A list of data from which to generate the operation
                 */
                virtual void doTransaction(const std::vector<std::pair<uint8_t *, int16_t>> &steps) = 0;
        };

        /**
         * \brief A temporary helper class which allows readable syntax for forming packets on a Bus
         */
        class BusTransaction {
            public:
                /**
                 * \brief Create a new, empty BusTransaction
                 * 
                 * \param[in,out] bus The bus on which the transaction is occurring
                 */
                BusTransaction(Bus &bus);

                /**
                 * \brief Commit the BusTransaction by performing it on the Bus
                 */
                ~BusTransaction();

                /**
                 * \brief Move-construct a BusTransaction with the signature of a copy-constructor
                 * 
                 * This allows returning copies of the BusTransaction from methods in Bus
                 * 
                 * \param[in,out] copy The original object
                 */
                BusTransaction(BusTransaction &copy);

                /**
                 * \brief Move-assign a BusTransaction with the signature of a copy-assignment
                 * 
                 * This allows returning copies of the BusTransaction from methods in Bus
                 * 
                 * \param[in,out] copy The original object
                 * 
                 * \return \c this
                 */
                BusTransaction &operator =(BusTransaction &copy);

                /**
                 * \brief Transmit an 8-bit field to the end of the BusTransaction
                 * 
                 * \param[in] data The data to transmit
                 * 
                 * \return The BusTransaction to allow \c << chaining
                 */
                BusTransaction &operator <<(uint8_t &data);
                
                /**
                 * \brief Transmit a 16-bit field to the end of the BusTransaction
                 * 
                 * \param[in] data The data to transmit
                 * 
                 * \return The BusTransaction to allow \c << chaining
                 */
                BusTransaction &operator <<(uint16_t &data);
                
                /**
                 * \brief Transmit a 32-bit field to the end of the BusTransaction
                 * 
                 * \param[in] data The data to transmit
                 * 
                 * \return The BusTransaction to allow \c << chaining
                 */
                BusTransaction &operator <<(uint32_t &data);

                /**
                 * \brief Receive an 8-bit field to the end of the BusTransaction
                 * 
                 * \param[out] data The data to receive
                 * 
                 * \return The BusTransaction to allow \c >> chaining
                 */
                BusTransaction &operator >>(uint8_t &data);

                /**
                 * \brief Receive a 16-bit field to the end of the BusTransaction
                 * 
                 * \param[out] data The data to receive
                 * 
                 * \return The BusTransaction to allow \c >> chaining
                 */
                BusTransaction &operator >>(uint16_t &data);

                /**
                 * \brief Receive a 32-bit field to the end of the BusTransaction
                 * 
                 * \param[out] data The data to receive
                 * 
                 * \return The BusTransaction to allow \c >> chaining
                 */
                BusTransaction &operator >>(uint32_t &data);

                /**
                 * \brief Receive a buffered field to the end of the BusTransaction
                 * 
                 * \param[out] data The data to receive
                 * \param[in] length The number of bytes to receive
                 * 
                 * \return The BusTransaction to allow chaining
                 */
                BusTransaction &read(uint8_t *data, uint16_t length);

                /**
                 * \brief Transmit a buffered field to the end of the BusTransaction
                 * 
                 * \param[in] data The data to transmit
                 * \param[in] length The number of bytes to transmit
                 * 
                 * \return The BusTransaction to allow chaining
                 */
                BusTransaction &write(uint8_t *data, uint16_t length);

            private:
                Bus &bus;
                std::vector<std::pair<uint8_t *, int16_t>> steps;
        };
    }
}

#endif
