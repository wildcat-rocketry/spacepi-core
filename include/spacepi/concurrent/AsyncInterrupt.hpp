#ifndef SPACEPI_CORE_CONCURRENT_ASYNCINTERRUPT_HPP
#define SPACEPI_CORE_CONCURRENT_ASYNCINTERRUPT_HPP

#include <mutex>
#include <spacepi/concurrent/Interrupt.hpp>

namespace spacepi {
    namespace concurrent {
        /**
         * \brief Base class for classes which can receive an asynchronous interrupt when the program cancellation is requested.
         * 
         * \see Interrupt
         */
        class AsyncInterrupt {
            friend class Interrupt;

            public:
                /**
                 * \brief Initializes the AsyncInterrupt
                 */
                AsyncInterrupt() noexcept;

                /**
                 * \brief Destroys the AsyncInterrupt
                 */
                virtual ~AsyncInterrupt();

                /**
                 * \brief Copy-construct the AsyncInterrupt
                 * 
                 * This provides no special functionality over AsyncInterrupt()
                 * 
                 * \param[in] copy The copy source
                 */
                AsyncInterrupt(const AsyncInterrupt &copy) noexcept;

                /**
                 * \brief Copy-assign the AsyncInterrupt
                 * 
                 * This does nothing
                 * 
                 * \param[in] copy The copy source
                 * 
                 * \return \c *this
                 */
                AsyncInterrupt &operator =(const AsyncInterrupt &copy) noexcept;

                /**
                 * \brief Move-construct the AsyncInterrupt
                 * 
                 * This provides no special functionality over AsyncInterrupt()
                 * 
                 * \param[in] move The move source
                 */
                AsyncInterrupt(const AsyncInterrupt &&move) noexcept;

                /**
                 * \brief Move-assign the AsyncInterrupt
                 * 
                 * This does nothing
                 * 
                 * \param[in] move The move source
                 * 
                 * \return \c *this
                 */
                AsyncInterrupt &operator =(const AsyncInterrupt &&move) noexcept;

            private:
                virtual void onCancel() noexcept = 0;

                static void fire() noexcept;

                static std::mutex mtx;
                static AsyncInterrupt *head;

                AsyncInterrupt *next;
                AsyncInterrupt *prev;
        };
    }
}

#endif
