#ifndef SPACEPI_CORE_CONCURRENT_SLEEP_HPP
#define SPACEPI_CORE_CONCURRENT_SLEEP_HPP

#include <chrono>
#include <boost/fiber/all.hpp>

namespace spacepi {
    namespace concurrent {
        class Sleep {
            public:
                template <typename Rep, typename Period>
                static void duration(const std::chrono::duration<Rep, Period> &duration) {
                    boost::this_fiber::sleep_for(duration);
                }

                static void momentarily() {
                    boost::this_fiber::yield();
                }

                template <typename Clock, typename Duration>
                static void until(const std::chrono::time_point<Clock, Duration> &time) {
                    boost::this_fiber::sleep_until(time);
                }

            private:
                Sleep() noexcept = default;
        };
    }
}

#endif
