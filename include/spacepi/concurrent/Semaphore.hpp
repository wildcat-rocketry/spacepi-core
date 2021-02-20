#ifndef SPACEPI_CORE_CONCURRENT_SEMAPHORE_HPP
#define SPACEPI_CORE_CONCURRENT_SEMAPHORE_HPP

#include <cstdint>

namespace spacepi {
    namespace concurrent {
        /**
         * \brief A semaphore built out of a mutex and a condition variable
         * 
         * \tparam Mutex The type of mutex to use (ex. \c std::mutex)
         * \tparam UniqueLock The type of lock to use (ex. \c std::unique_lock<std::mutex>)
         * \tparam ConditionVariable The type of condition variable to use (ex. \c std::condition_variable)
         */
        template <typename Mutex, typename UniqueLock, typename ConditionVariable>
        class Semaphore {
            public:
                /**
                 * \brief Initialize a new semaphore
                 * 
                 * \param[in] locks The number of locks initially available to the Semaphore
                 */
                Semaphore(uint32_t locks = 0) noexcept : locksAvailable(locks), locksRequested(0), locksServed(0) {
                }

                /**
                 * \brief Destroy a Semaphore
                 */
                virtual ~Semaphore() = default;

                Semaphore(Semaphore &) = delete;
                Semaphore &operator =(Semaphore &) = delete;

                /**
                 * \brief Obtain \c locks locks from the Semaphore
                 * 
                 * \param[in] locks The number of locks to obtain
                 */
                void lock(uint32_t locks = 1) {
                    UniqueLock lck(mutex);
                    doLock(locks, lck);
                }

                /**
                 * \brief Attempt to obtain \c locks locks from the Semaphore, but do not block if operation is not possible
                 * 
                 * \param[in] locks The number of locks to attempt to obtain
                 */
                bool try_lock(uint32_t locks = 1) {
                    UniqueLock lck(mutex);
                    bool can = locks <= locksAvailable;
                    if (can) {
                        doLock(locks, lck);
                    }
                    return can;
                }

                /**
                 * \brief Release \c locks locks back into the Semaphore
                 * 
                 * \param[in] locks The number of locks to release
                 */
                void unlock(uint32_t locks = 1) {
                    UniqueLock lck(mutex);
                    locksAvailable += locks;
                    condition.notify_all();
                }

            private:
                void doLock(uint32_t locks, UniqueLock &lck) {
                    uint32_t lockId = locksRequested++;
                    while (lockId != locksServed || locks > locksAvailable) {
                        condition.wait(lck);
                    }
                    ++locksServed;
                    locksAvailable -= locks;
                }

                Mutex mutex;
                ConditionVariable condition;
                uint32_t locksAvailable;
                uint32_t locksRequested;
                uint32_t locksServed;
        };
    }
}

#endif
