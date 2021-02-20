#ifndef SPACEPI_CORE_CONCURRENT_RWMUTEX_HPP
#define SPACEPI_CORE_CONCURRENT_RWMUTEX_HPP

#include <cstdint>
#include <limits>
#include <spacepi/concurrent/Semaphore.hpp>

namespace spacepi {
    namespace concurrent {
        /**
         * \brief A mutex which allows multiple concurrent reads while still locking against concurrent writes or read/writes.
         * 
         * Either an infinite number of locks can be obtained for reading concurrently, or a single write lock can be
         * obtained at any given time.  Additionally, locks are served in a FIFO order, so when a write is attempted,
         * all further read operations block until that write has completed, even if the write does not occur
         * immediately due to other reads still being in progress.
         * 
         * \tparam Mutex The type of mutex to use (ex. \c std::mutex)
         * \tparam UniqueLock The type of lock to use (ex. \c std::unique_lock<std::mutex>)
         * \tparam ConditionVariable The type of condition variable to use (ex. \c std::condition_variable)
         */
        template <typename Mutex, typename UniqueLock, typename ConditionVariable>
        class RWMutex : private Semaphore<Mutex, UniqueLock, ConditionVariable> {
            public:
                /**
                 * \brief Helper class that allows locking the RWMutex for reads using \c std::unique_lock
                 */
                class ReadSide final {
                    friend class RWMutex;

                    public:
                        ReadSide(ReadSide &) = delete;
                        ReadSide &operator =(ReadSide &) = delete;

                        /**
                         * \brief Lock the RWMutex for reading
                         */
                        void lock() {
                            mutex.lockRead();
                        }

                        /**
                         * \brief Unlock the RWMutex for reading
                         */
                        void unlock() {
                            mutex.unlockRead();
                        }

                    private:
                        explicit ReadSide(RWMutex<Mutex, UniqueLock, ConditionVariable> &mutex) : mutex(mutex) {
                        }

                        RWMutex<Mutex, UniqueLock, ConditionVariable> &mutex;
                };

                /**
                 * \brief Helper class that allows locking the RWMutex for writes using \c std::unique_lock
                 */
                class WriteSide final {
                    friend class RWMutex;

                    public:
                        WriteSide(WriteSide &) = delete;
                        WriteSide &operator =(WriteSide &) = delete;

                        /**
                         * \brief Lock the RWMutex for writing
                         */
                        void lock() {
                            mutex.lockWrite();
                        }

                        /**
                         * \brief Unlock the RWMutex for writing
                         */
                        void unlock() {
                            mutex.unlockWrite();
                        }

                    private:
                        explicit WriteSide(RWMutex<Mutex, UniqueLock, ConditionVariable> &mutex) : mutex(mutex) {
                        }

                        RWMutex<Mutex, UniqueLock, ConditionVariable> &mutex;
                };

                /**
                 * \brief Initialize a RWMutex with no locks
                 */
                RWMutex() : Semaphore<Mutex, UniqueLock, ConditionVariable>(std::numeric_limits<uint32_t>::max()), readSide(*this), writeSide(*this) {
                }

                RWMutex(RWMutex<Mutex, UniqueLock, ConditionVariable> &) = delete;
                RWMutex<Mutex, UniqueLock, ConditionVariable> &operator =(RWMutex<Mutex, UniqueLock, ConditionVariable> &) = delete;

                /**
                 * \brief Lock the RWMutex for reading
                 */
                void lockRead() {
                    this->lock(1);
                }

                /**
                 * \brief Lock the RWMutex for writing
                 */
                void lockWrite() {
                    this->lock(std::numeric_limits<uint32_t>::max());
                }

                /**
                 * \brief Unlock the RWMutex for reading
                 */
                void unlockRead() {
                    this->unlock(1);
                }

                /**
                 * \brief Unlock the RWMutex for writing
                 */
                void unlockWrite() {
                    this->unlock(std::numeric_limits<uint32_t>::max());
                }

                /**
                 * \brief Obtain a helper object that allows locking the RWMutex for reading using as \c std::unique_lock
                 * 
                 * \return The helper object
                 * 
                 * \code{.cpp}
                 * #include <mutex>
                 * #include <spacepi/concurrent/RWMutex.hpp>
                 * 
                 * using namespace std;
                 * using namespace spacepi::concurrent;
                 * 
                 * RWMutex mtx;
                 * unique_lock<RWMutex::ReadSide> lck(mtx.read());
                 * \endcode
                 */
                ReadSide &read() {
                    return readSide;
                }

                /**
                 * \brief Obtain a helper object that allows locking the RWMutex for writing using an \c std::unique_lock
                 * 
                 * \return The helper object
                 * 
                 * \code{.cpp}
                 * #include <mutex>
                 * #include <spacepi/concurrent/RWMutex.hpp>
                 * 
                 * using namespace std;
                 * using namespace spacepi::concurrent;
                 * 
                 * RWMutex mtx;
                 * unique_lock<RWMutex::WriteSide> lck(mtx.write());
                 * \endcode
                 */
                WriteSide &write() {
                    return writeSide;
                }

            private:
                ReadSide readSide;
                WriteSide writeSide;
        };
    }
}

#endif
