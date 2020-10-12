#ifndef SPACEPI_CORE_CONCURRENT_SEMAPHORE_HPP
#define SPACEPI_CORE_CONCURRENT_SEMAPHORE_HPP

#include <cstdint>

namespace spacepi {
    namespace concurrent {
        template <typename Mutex, typename UniqueLock, typename ConditionVariable>
        class Semaphore {
            public:
                Semaphore(uint32_t locks = 0) : locksAvailable(locks), locksRequested(0), locksServed(0) {
                }

                Semaphore(const Semaphore &) = delete;

                Semaphore &operator =(const Semaphore &) = delete;

                void lock(uint32_t locks = 1) {
                    UniqueLock lck(mutex);
                    do_lock(locks, lck);
                }

                bool try_lock(uint32_t locks = 1) {
                    UniqueLock lck(mutex);
                    bool can = locks <= locksAvailable;
                    if (can) {
                        do_lock(locks, lck);
                    }
                    return can;
                }

                void unlock(uint32_t locks = 1) {
                    UniqueLock lck(mutex);
                    locksAvailable += locks;
                    condition.notify_all();
                }

            private:
                void do_lock(uint32_t locks, UniqueLock &lck) {
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
