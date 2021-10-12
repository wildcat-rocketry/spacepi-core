#ifndef SPACEPI_CORE_CONCURRENT_LOCK_HPP
#define SPACEPI_CORE_CONCURRENT_LOCK_HPP

#include <spacepi/concurrent/Mutex.hpp>
#include <spacepi/concurrent/SleepMode.hpp>

namespace spacepi {
    namespace concurrent {
        /**
         * \private
         */
        constexpr class TryLockType {
        } TryLock;

        /**
         * \private
         */
        constexpr class DeferLockType {
        } DeferLock;

        /**
         * \private
         */
        constexpr class AdoptLockType {
        } AdoptLock;

        /**
         * \brief Helper class for locking a Mutex with RAII
         *
         * \tparam Mode Type of algorithms used to sleep while waiting for synchronization
         */
        template <SleepMode Mode>
        class Lock {
            public:
                /**
                 * \brief Constructs a Lock and locks the Mutex
                 *
                 * \param[in] mtx The Mutex to lock
                 */
                inline Lock(Mutex<Mode> &mtx) noexcept
                    : mtx(&mtx), have(true) {
                    mtx.lock();
                }

                /**
                 * \brief Constructs a Lock and attempts to lock the Mutex without blocking
                 *
                 * \param[in] mtx The Mutex to lock
                 * \param[in] tag \c TryLock
                 */
                inline Lock(Mutex<Mode> &mtx, const TryLockType &tag) noexcept
                    : mtx(&mtx), have(mtx.tryLock()) {
                }

                /**
                 * \brief Constructs a Lock without locking the Mutex
                 *
                 * \param[in] mtx The Mutex to lock
                 * \param[in] tag \c DeferLock
                 */
                constexpr Lock(Mutex<Mode> &mtx, const DeferLockType &tag) noexcept
                    : mtx(&mtx), have(false) {
                }

                /**
                 * \brief Constructs a Lock and assumes the calling code has already locked the Mutex
                 *
                 * \param[in] mtx The Mutex to lock
                 * \param[in] tag \c AdoptLock
                 */
                constexpr Lock(Mutex<Mode> &mtx, const AdoptLockType &tag) noexcept
                    : mtx(&mtx), have(true) {
                }

                /**
                 * \brief Move-constructs a Lock
                 *
                 * \param[in] move The old Lock
                 */
                constexpr Lock(Lock &&move) noexcept
                    : mtx(move.mtx), have(move.have) {
                    move.release();
                }

                /**
                 * \brief Move-assigns a Lock
                 *
                 * \param[in] move The old Lock
                 * \return \c *this
                 */
                constexpr Lock &operator=(Lock &&move) noexcept {
                    unlock();
                    mtx = move.mtx;
                    have = move.have;
                    move.release();
                }

                /**
                 * \brief Unlocks the Mutex if it was locked by this Lock
                 */
                inline ~Lock() noexcept {
                    unlock();
                }

                /**
                 * \brief Locks the Mutex if it is not already locked by this Lock
                 */
                inline void lock() noexcept {
                    if (!have) {
                        mtx->lock();
                        adopt();
                    }
                }

                /**
                 * \brief Attempts to lock the Mutex without blocking if it is not already locked by this Lock
                 *
                 * \return \c true if the Mutex is now locked by this Lock, otherwise \c false
                 */
                inline bool tryLock() noexcept {
                    if (have) {
                        return true;
                    }
                    return have = mtx->tryLock();
                }

                /**
                 * \brief Unlocks the Mutex if it was locked by this Lock
                 */
                inline void unlock() noexcept {
                    if (have) {
                        release();
                        mtx->unlock();
                    }
                }

                /**
                 * \brief Take ownership of the lock on the Mutex without locking the Mutex
                 */
                constexpr void adopt() noexcept {
                    have = true;
                }

                /**
                 * \brief Release ownership of the lock on the Mutex without unlocking the Mutex
                 */
                constexpr void release() noexcept {
                    have = false;
                }

                /**
                 * \brief Determine if this Lock has locked the Mutex
                 *
                 * \return \c true if this Lock has locked the Mutex, otherwise \c false
                 */
                constexpr operator bool() const noexcept {
                    return have;
                }

            private:
                Mutex<Mode> *mtx;
                bool have;
        };
    }
}

#endif
