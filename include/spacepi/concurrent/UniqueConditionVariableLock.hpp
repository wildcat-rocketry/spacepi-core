#ifndef SPACEPI_CORE_CONCURRENT_UNIQUECONDITIONVARIABLELOCK_HPP
#define SPACEPI_CORE_CONCURRENT_UNIQUECONDITIONVARIABLELOCK_HPP

#include <chrono>
#include <mutex>
#include <utility>
#include <spacepi/concurrent/ConditionVariable.hpp>

namespace spacepi {
    namespace concurrent {
        /**
         * \brief Wrapper around \c std::unique_lock<ConditionVariable> to add support for the ThreadPool and for
         * integrated cancellation points (see Interrupt)
         * 
         * \see ConditionVariable
         */
        class UniqueConditionVariableLock {
            public:
                /**
                 * \brief Same as \c std::unique_lock<Mutex>::unique_lock()
                 */
                UniqueConditionVariableLock() noexcept = default;

                /**
                 * \brief Same as \c std::unique_lock<Mutex>::unique_lock(Mutex \c &)
                 */
                UniqueConditionVariableLock(ConditionVariable &mtx);

                /**
                 * \brief Same as \c std::unique_lock<Mutex>::unique_lock(Mutex \c &, \c std::try_to_lock_t)
                 */
                UniqueConditionVariableLock(ConditionVariable &mtx, std::try_to_lock_t tag);

                /**
                 * \brief Same as \c std::unique_lock<Mutex>::unique_lock(Mutex \c &, \c std::defer_lock_t)
                 */
                UniqueConditionVariableLock(ConditionVariable &mtx, std::defer_lock_t tag);

                /**
                 * \brief Same as \c std::unique_lock<Mutex>::unique_lock(Mutex \c &, \c std::adopt_lock_t)
                 */
                UniqueConditionVariableLock(ConditionVariable &mtx, std::adopt_lock_t tag);

                /**
                 * \brief Same as \c std::unique_lock<Mutex>::unique_lock<Rep, \c Period>(Mutex \c &, \c const \c std::chrono::duration<Rep, \c Period> \c &)
                 */
                template <typename Rep, typename Period>
                UniqueConditionVariableLock(ConditionVariable &mtx, const std::chrono::duration<Rep, Period> &relTime) : UniqueConditionVariableLock(mtx, std::defer_lock) {
                    try_lock_for(relTime);
                }

                /**
                 * \brief Same as \c std::unique_lock<Mutex>::unique_lock<Clock, \c Duration>(Mutex \c &, \c const \c std::chrono::time_point<Clock, \c Duration> \c &)
                 */
                template <typename Clock, typename Duration>
                UniqueConditionVariableLock(ConditionVariable &mtx, const std::chrono::time_point<Clock, Duration> &absTime) : UniqueConditionVariableLock(mtx, std::defer_lock) {
                    try_lock_until(absTime);
                }

                /**
                 * \brief Same as \c std::unique_lock<Mutex>::~unique_lock()
                 */
                virtual ~UniqueConditionVariableLock() = default;

                UniqueConditionVariableLock(UniqueConditionVariableLock &) = delete;
                UniqueConditionVariableLock &operator =(UniqueConditionVariableLock &) = delete;

                /**
                 * \brief Same as \c std::unique_lock<Mutex>::unique_lock(std::unique_lock<Mutex> \c &&)
                 */
                UniqueConditionVariableLock(UniqueConditionVariableLock &&move);

                /**
                 * \brief Same as \c std::unique_lock<Mutex>::operator \c =(std::unique_lock<Mutex> \c &&)
                 */
                UniqueConditionVariableLock &operator =(UniqueConditionVariableLock &&move);

                /**
                 * \brief Same as \c std::unique_lock<Mutex>::lock()
                 */
                void lock();

                /**
                 * \brief Same as \c std::unique_lock<Mutex>::try_lock()
                 */
                bool try_lock();

                /**
                 * \brief Same as \c std::unique_lock<Mutex>::try_lock_for<Rep, \c Period>(const \c std::chrono::duration<Rep, \c Period> \c &)
                 */
                template <typename Rep, typename Period>
                bool try_lock_for(const std::chrono::duration<Rep, Period> &relTime) {
                    return try_lock_until(std::chrono::steady_clock::now() + relTime);
                }

                /**
                 * \brief Same as \c std::unique_lock<Mutex>::try_lock_until<Clock, \c Duration>(const \c std::chrono::time_point<Clock, \c Duration> \c &)
                 */
                template <typename Clock, typename Duration>
                bool try_lock_until(const std::chrono::time_point<Clock, Duration> &absTime) {
                    if (slck.mutex()) {
                        return slck.try_lock_until(absTime);
                    } else if (blck.mutex()) {
                        return blck.try_lock_until(absTime);
                    }
                }

                /**
                 * \brief Same as \c std::unique_lock<Mutex>::unlock()
                 */
                void unlock();

                /**
                 * \brief Gets the current mutex which is owned by this UniqueConditionVariableLock
                 * 
                 * Only one of the parameters in the returned \c pair will ever be non-null.
                 * 
                 * \return The two possible mutex types this UniqueConditionVariableLock owns
                 */
                std::pair<std::mutex *, boost::fibers::mutex *> mutex() const noexcept;

                /**
                 * \brief Get the wrapped \c std::unique_lock<std::mutex>
                 * 
                 * \return The wrapped object
                 */
                operator std::unique_lock<std::mutex> &() noexcept;

                /**
                 * \brief Get the wrapped \c std::unique_lock<boost::fibers::mutex>
                 * 
                 * \return The wrapped object
                 */
                operator std::unique_lock<boost::fibers::mutex> &() noexcept;

                /**
                 * \brief Same as \c std::unique_lock<Mutex>::owns_lock()
                 */
                bool owns_lock() const noexcept;

                /**
                 * \brief Same as \c std::unique_lock<Mutex>::operator \c bool()
                 */
                explicit operator bool() const noexcept;

                /**
                 * \brief Same as \c std::unique_lock<Mutex>::release()
                 */
                std::pair<std::mutex *, boost::fibers::mutex *> release() noexcept;

            private:
                std::unique_lock<std::mutex> slck;
                std::unique_lock<boost::fibers::mutex> blck;
        };
    }
}

#endif
