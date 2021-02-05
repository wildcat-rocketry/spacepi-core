#ifndef SPACEPI_CORE_CONCURRENT_CONDITIONVARIABLE_HPP
#define SPACEPI_CORE_CONCURRENT_CONDITIONVARIABLE_HPP

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <utility>
#include <boost/fiber/all.hpp>
#include <spacepi/concurrent/Interrupt.hpp>
#include <spacepi/util/Exception.hpp>

namespace spacepi {
    namespace concurrent {
        class UniqueConditionVariableLock;

        /**
         * \brief Wrapper around \c std::condition_variable paired with a \c std::mutex to add support for the
         * ThreadPool and for integrated cancellation points (see Interrupt)
         * 
         * \see UniqueConditionVariableLock
         */
        class ConditionVariable {
            public:
                /**
                 * \brief Creates a new ConditionVariable
                 */
                ConditionVariable() noexcept;

                /**
                 * \brief Destroys the ConditionVariable
                 */
                ~ConditionVariable() noexcept;

                ConditionVariable(ConditionVariable &) = delete;
                ConditionVariable &operator =(ConditionVariable &) = delete;

                /**
                 * \brief Same as \c std::condition_variable::notify_all()
                 */
                void notify_all() noexcept;

                /**
                 * \brief Same as \c std::condition_variable::notify_one()
                 */
                void notify_one() noexcept;

                /**
                 * \brief Same as \c std::condition_variable::wait<Mutex>(std::unique_lock<Mutex> \c &)
                 * 
                 * This method is a cancellation point (see Interrupt).
                 */
                void wait(UniqueConditionVariableLock &lck);

                /**
                 * \brief Same as \c std::condition_variable::wait<Mutex, \c Predicate>(std::unique_lock<Mutex> \c &, \c Predicate)
                 * 
                 * This method is a cancellation point (see Interrupt).
                 */
                template <typename Predicate>
                void wait(UniqueConditionVariableLock &lck, Predicate pred) {
                    if (pred()) {
                        Interrupt::cancellationPoint();
                    } else {
                        do {
                            wait(lck);
                        } while (!pred());
                    }
                }

                /**
                 * \brief Same as \c std::condition_variable::wait_for<Mutex, \c Rep, \c Period>(std::unique_lock<Mutex> \c &, \c const \c std::chrono::duration<Rep, \c Period> \c &)
                 * 
                 * This method is a cancellation point (see Interrupt).
                 */
                template <typename Rep, typename Period>
                std::cv_status wait_for(UniqueConditionVariableLock &lck, const std::chrono::duration<Rep, Period> &relTime) {
                    return wait_until(lck, std::chrono::steady_clock::now() + relTime);
                }

                /**
                 * \brief Same as \c std::condition_variable::wait_for<Mutex, \c Rep, \c Period, \c Predicate>(std::unique_lock<Mutex> \c &, \c const \c std::chrono::duration<Rep, \c Period> \c &, \c Predicate)
                 * 
                 * This method is a cancellation point (see Interrupt).
                 */
                template <typename Rep, typename Period, typename Predicate>
                bool wait_for(UniqueConditionVariableLock &lck, const std::chrono::duration<Rep, Period> &relTime, Predicate pred) {
                    return wait_until(lck, std::chrono::steady_clock::now() + relTime, pred);
                }

                /**
                 * \brief Same as \c std::condition_variable::wait_until<Mutex, \c Clock, \c Duration>(std::unique_lock<Mutex> \c &, \c const \c std::chrono::time_point<Clock, \c Duration> \c &)
                 * 
                 * This method is a cancellation point (see Interrupt).
                 */
                template <typename Clock, typename Duration>
                std::cv_status wait_until(UniqueConditionVariableLock &lck, const std::chrono::time_point<Clock, Duration> &absTime) {
                    Interrupt::cancellationPoint();
                    if (boost::this_fiber::get_id() == boost::fibers::fiber::id()) {
                        return scond.wait_until(lck, absTime);
                    } else {
                        boost::fibers::cv_status s;
                        ++bwait;
                        try {
                            s = bcond.wait_until(lck, absTime);
                            --bwait;
                        } catch (...) {
                            --bwait;
                            throw;
                        }
                        switch (s) {
                            case boost::fibers::cv_status::no_timeout:
                                return std::cv_status::no_timeout;
                            case boost::fibers::cv_status::timeout:
                                return std::cv_status::timeout;
                            default:
                                throw EXCEPTION(spacepi::util::ArgumentException("Unknown boost::fibers::cv_status constant."));
                        }
                    }
                }

                /**
                 * \brief Same as \c std::condition_variable::wait_until<Mutex, \c Clock, \c Duration, \c Predicate>(std::unique_lock<Mutex> \c &, \c const \c std::chrono::time_point<Clock, \c Duration> \c &, \c Predicate)
                 * 
                 * This method is a cancellation point (see Interrupt).
                 */
                template <typename Clock, typename Duration, typename Predicate>
                bool wait_until(UniqueConditionVariableLock &lck, const std::chrono::time_point<Clock, Duration> &absTime, Predicate pred) {
                    if (pred()) {
                        Interrupt::cancellationPoint();
                    } else {
                        do {
                            if (wait_until(lck, absTime) == std::cv_status::timeout) {
                                return pred();
                            }
                        } while (!pred());
                    }
                    return true;
                }

                /**
                 * \brief Same as \c std::mutex::lock()
                 */
                void lock();

                /**
                 * \brief Same as \c std::mutex::try_lock()
                 */
                bool try_lock();

                /**
                 * \brief Same as \c std::mutex::unlock()
                 */
                void unlock();

                /**
                 * \brief Gets the current mutex which is wrapped by this ConditionVariable in this threading context
                 * 
                 * Only one of the parameters in the returned \c pair will ever be non-null.
                 * 
                 * \return The two possible mutex types this ConditionVariable wraps
                 */
                std::pair<std::mutex *, boost::fibers::mutex *> mutex() noexcept;

                /**
                 * \brief Equivalent to calling notify_all() on all ConditionVariable's currently allocated in the
                 * program
                 */
                static void notify_global() noexcept;

            private:
                static std::mutex gmtx;
                static ConditionVariable *head;

                ConditionVariable *next;
                ConditionVariable *prev;
                std::mutex smtx;
                boost::fibers::mutex bmtx;
                std::condition_variable scond;
                boost::fibers::condition_variable bcond;
                int bwait;
        };
    }
}

#endif
