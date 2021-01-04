#ifndef SPACEPI_CORE_CONCURRENT_SLEEP_HPP
#define SPACEPI_CORE_CONCURRENT_SLEEP_HPP

#include <chrono>
#include <boost/fiber/all.hpp>

namespace spacepi {
    namespace concurrent {
        /**
         * \brief A set of \c static methods which allow sleeping when using a ThreadPool
         */
        class Sleep {
            public:
                /**
                 * \brief Sleep for at least a given duration
                 * 
                 * \param[in] duration The minimum amount of time to sleep for
                 */
                template <typename Rep, typename Period>
                static void duration(const std::chrono::duration<Rep, Period> &duration) {
                    boost::this_fiber::sleep_for(std::chrono::duration_cast<std::chrono::nanoseconds>(duration));
                }

                /**
                 * \brief Marks a location at which it is okay for the scheduler to switch tasks
                 * 
                 * This should not be called inside a critical section, as that would allow race conditions to occur.
                 */
                static void momentarily() {
                    boost::this_fiber::yield();
                }

                /**
                 * \brief Sleep at least until a certain point in time
                 * 
                 * \param[in] time The earliest time this method may return
                 */
                template <typename Clock, typename Duration>
                static void until(const std::chrono::time_point<Clock, Duration> &time) {
                    boost::this_fiber::sleep_until(std::chrono::time_point_cast<std::chrono::nanoseconds>(time));
                }

            private:
                Sleep() noexcept = default;
        };
    }
}

#endif
