#ifndef SPACEPI_CORE_CONCURRENT_MUTEX_HPP
#define SPACEPI_CORE_CONCURRENT_MUTEX_HPP

#include <spacepi/concurrent/SleepMode.hpp>
#include <spacepi/Platform.hpp>

namespace spacepi {
    namespace concurrent {
        /**
         * \private
         */
        template <SleepMode Mode>
        class Mutex {
            public:
                Mutex() = delete;
        };

        /**
         * \brief Tiny mutex implementation to be used with small critical sections
         *
         * This mutex expects critical sections to execute quickly, so it does not give up the processor to another
         * thread while waiting for a lock.
         */
        template <>
        class Mutex<Fast> {
            public:
                /**
                 * \brief Constructs a Mutex
                 */
                constexpr Mutex() noexcept
                    : val(0) {
                }

                Mutex(Mutex &) = delete;
                Mutex &operator=(Mutex &) = delete;

                /**
                 * \brief Attempts to lock the Mutex without blocking
                 *
                 * \return \c true if the Mutex was locked, otherwise \c false
                 */
                inline bool tryLock() noexcept {
                    return !Platform::testAndSet(val);
                }

                /**
                 * \brief Locks the Mutex
                 *
                 * This method will block without sleeping until the Mutex can be locked
                 */
                inline void lock() noexcept {
                    while (!tryLock())
                        ;
                }

                /**
                 * \brief Unlocks the Mutex
                 */
                inline void unlock() noexcept {
                    Platform::unset(val);
                }

            private:
                volatile int val;
        };
    }
}

#endif
