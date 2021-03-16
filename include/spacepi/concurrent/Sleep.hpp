#ifndef SPACEPI_CORE_CONCURRENT_SLEEP_HPP
#define SPACEPI_CORE_CONCURRENT_SLEEP_HPP

#include <chrono>
#include <boost/fiber/all.hpp>
#include <spacepi/concurrent/ConditionVariable.hpp>
#include <spacepi/concurrent/Interrupt.hpp>
#include <spacepi/concurrent/UniqueConditionVariableLock.hpp>

namespace spacepi {
    namespace concurrent {
        namespace detail {
            class SleepPredicate final {
                public:
                    static SleepPredicate instance;

                    bool operator ()() const noexcept;
            };
        }

        /**
         * \brief A set of \c static methods which allow sleeping when using a ThreadPool
         */
        class Sleep final {
            public:
                /**
                 * \brief Sleep for at least a given duration
                 * 
                 * \param[in] duration The minimum amount of time to sleep for
                 * 
                 * This method is a cancellation point (see spacepi::concurrent::Interrupt).
                 */
                template <typename Rep, typename Period>
                static void duration(const std::chrono::duration<Rep, Period> &duration) {
                    UniqueConditionVariableLock lck(cond);
                    cond.wait_for(lck, duration, detail::SleepPredicate::instance);
                }

                /**
                 * \brief Marks a location at which it is okay for the scheduler to switch tasks
                 * 
                 * This should not be called inside a critical section, as that would allow race conditions to occur.
                 * 
                 * This method is a cancellation point (see spacepi::concurrent::Interrupt).
                 */
                static inline void momentarily() {
                    Interrupt::cancellationPoint();
                    boost::this_fiber::yield();
                    Interrupt::cancellationPoint();
                }

                /**
                 * \brief Sleep at least until a certain point in time
                 * 
                 * \param[in] time The earliest time this method may return
                 * 
                 * This method is a cancellation point (see spacepi::concurrent::Interrupt).
                 */
                template <typename Clock, typename Duration>
                static void until(const std::chrono::time_point<Clock, Duration> &time) {
                    UniqueConditionVariableLock lck(cond);
                    cond.wait_until(lck, time, detail::SleepPredicate::instance);
                }

                /**
                 * \brief Sleep until the program is interrupted
                 * 
                 * This method will never return normally, but may throw an exception to exit.
                 * 
                 * This method is a cancellation point (see spacepi::concurrent::Interrupt).
                 */
                static void forever();

            private:
                Sleep() noexcept = default;

                static ConditionVariable cond;
        };
    }
}

#endif
