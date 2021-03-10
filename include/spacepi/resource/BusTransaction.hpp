#ifndef SPACEPI_CORE_RESOURCE_BUSTRANSACTION_HPP
#define SPACEPI_CORE_RESOURCE_BUSTRANSACTION_HPP

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

namespace spacepi {
    namespace resource {
        /**
         * \brief Base class for Bus and BusTransaction
         * 
         * \tparam TReturn The return type of every method
         */
        template <typename TReturn>
        class BusBase {
            public:
                /**
                 * \brief Destroys a BusBase
                 */
                virtual ~BusBase() noexcept(false);

                /**
                 * \brief Transmit an 8-bit field to the end of the BusTransaction
                 * 
                 * \param[in] data The data to transmit
                 * 
                 * \return The BusTransaction to allow \c << chaining
                 */
                TReturn operator <<(uint8_t data) noexcept;

                /**
                 * \brief Helper function for stream modifiers
                 * 
                 * \param[in] func The stream modifier
                 * 
                 * \return The BusTransaction to allow \c << chaining
                 */
                template <typename Func>
                TReturn operator <<(Func func) noexcept {
                    return func(*this);
                }

                /**
                 * \brief Sends a sequence of don't care bytes at the end of the BusTransaction
                 * 
                 * \param[in] count The number of don't care bytes to transmit
                 * 
                 * \return The BusTransaction to allow \c << chaining
                 */
                TReturn seek(int count) noexcept;

                /**
                 * \brief Transmit a buffered field to the end of the BusTransaction
                 * 
                 * \param[in] data The data to transmit
                 * \param[in] length The number of bytes to transmit
                 * 
                 * \return The BusTransaction to allow chaining
                 */
                virtual TReturn write(const uint8_t *data, uint16_t length) noexcept = 0;

                /**
                 * \brief Receive an 8-bit field to the end of the BusTransaction
                 * 
                 * \param[out] data The data to receive
                 * 
                 * \return The BusTransaction to allow \c >> chaining
                 */
                TReturn operator >>(uint8_t &data) noexcept;

                /**
                 * \brief Helper function for stream modifiers
                 * 
                 * \param[in] func The stream modifier
                 * 
                 * \return The BusTransaction to allow \c >> chaining
                 */
                template <typename Func>
                TReturn operator >>(Func func) noexcept {
                    return func(*this);
                }

                /**
                 * \brief Ignore a sequence of bytes from the end of the BusTransaction
                 * 
                 * \param[in] count The number of bytes to ignore
                 * 
                 * \return The BusTransaction to allow \c >> chaining
                 */
                TReturn skip(int count) noexcept;

                /**
                 * \brief Receive a buffered field to the end of the BusTransaction
                 * 
                 * \param[out] data The data to receive
                 * \param[in] length The number of bytes to receive
                 * 
                 * \return The BusTransaction to allow chaining
                 */
                virtual TReturn read(uint8_t *data, uint16_t length) noexcept = 0;

            private:
                virtual TReturn ref() noexcept = 0;

                template <typename Func>
                TReturn seekSkip(Func func, int count);
        };

        class BusTransaction;

        /**
         * \brief Base class for hardware resources which can send or recieve packets as a stream of data types
         */
        class Bus : public BusBase<BusTransaction> {
            friend class BusTransaction;

            public:
                /**
                 * \copydoc BusBase::read
                 */
                BusTransaction read(uint8_t *data, uint16_t length) noexcept;

                /**
                 * \copydoc BusBase::write
                 */
                BusTransaction write(const uint8_t *data, uint16_t length) noexcept;

            private:
                BusTransaction ref() noexcept;

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
        class BusTransaction final : public BusBase<BusTransaction &> {
            public:
                /**
                 * \brief Create a new, empty BusTransaction
                 * 
                 * \param[in,out] bus The bus on which the transaction is occurring
                 */
                BusTransaction(Bus &bus) noexcept;

                /**
                 * \brief Commit the BusTransaction by performing it on the Bus
                 */
                ~BusTransaction() noexcept(false);

                /**
                 * \copydoc BusBase::read
                 */
                BusTransaction &read(uint8_t *data, uint16_t length) noexcept;

                /**
                 * \copydoc BusBase::write
                 */
                BusTransaction &write(const uint8_t *data, uint16_t length) noexcept;

            private:
                BusTransaction &ref() noexcept;

                Bus *bus;
                std::shared_ptr<std::vector<std::pair<uint8_t *, int16_t>>> steps;
                std::shared_ptr<std::vector<std::vector<uint8_t>>> alloc;
        };

        /**
         * \brief Stream modifier to send a sequence of don't care bytes at the end of the BusTransaction
         */
        class seek final {
            public:
                /**
                 * \brief Initialize the seek modifier
                 * 
                 * \param[in] count The number of don't care bytes to transmit
                 */
                seek(int count) noexcept;

                /**
                 * \brief Performs the operation on a stream
                 * 
                 * \tparam Target The return type of the function
                 * 
                 * \param[in,out] os The stream to perform on
                 * 
                 * \return The BusTransaction to allow \c << chaining
                 */
                template <typename Target>
                Target operator ()(BusBase<Target> &os) const noexcept;

            private:
                int count;
        };

        /**
         * \brief Stream modifier to transmit a buffered field to the end of the BusTransaction
         */
        class write final {
            public:
                /**
                 * \brief Initialize the write modifier
                 * 
                 * \param[in] data The data to transmit
                 * \param[in] length The number of bytes to transmit
                 */
                write(const uint8_t *data, uint16_t length) noexcept;

                /**
                 * \brief Performs the operation on a stream
                 * 
                 * \tparam Target The return type of the function
                 * 
                 * \param[in,out] os The stream to perform on
                 * 
                 * \return The BusTransaction to allow \c << chaining
                 */
                template <typename Target>
                Target operator ()(BusBase<Target> &os) const noexcept;

            private:
                const uint8_t *data;
                uint16_t length;
        };

        /**
         * \brief Stream modifier to ignore a sequence of bytes from the end of the BusTransaction
         */
        class skip final {
            public:
                /**
                 * \brief Initialize the skip modifier
                 * 
                 * \param[in] count The number of bytes to ignore
                 */
                skip(int count) noexcept;

                /**
                 * \brief Performs the operation on a stream
                 * 
                 * \tparam Target The return type of the function
                 * 
                 * \param[in,out] os The stream to perform on
                 * 
                 * \return The BusTransaction to allow \c >> chaining
                 */
                template <typename Target>
                Target operator ()(BusBase<Target> &os) const noexcept;

            private:
                int count;
        };

        /**
         * \brief Stream modifier to receive a buffered field to the end of the BusTransaction
         */
        class read final {
            public:
                /**
                 * \brief Initialize the read modifier
                 * 
                 * \param[out] data The data to receive
                 * \param[in] length The number of bytes to receive
                 */
                read(uint8_t *data, uint16_t length) noexcept;

                /**
                 * \brief Performs the operation on a stream
                 * 
                 * \tparam Target The return type of the function
                 * 
                 * \param[in,out] os The stream to perform on
                 * 
                 * \return The BusTransaction to allow \c >> chaining
                 */
                template <typename Target>
                Target operator ()(BusBase<Target> &os) const noexcept;

            private:
                uint8_t *data;
                uint16_t length;
        };
    }
}

#endif
