#ifndef SPACEPI_CORE_CONCURRENT_INTERRUPT_HPP
#define SPACEPI_CORE_CONCURRENT_INTERRUPT_HPP

#include <condition_variable>
#include <csignal>
#include <mutex>
#include <set>
#include <thread>
#include <boost/fiber/all.hpp>
#include <spacepi/util/Exception.hpp>

namespace spacepi {
    namespace concurrent {
        namespace detail {
            class ThreadID final {
                public:
                    ThreadID() noexcept;

                    bool operator <(const ThreadID &other) const noexcept;

                private:
                    std::thread::id tid;
                    boost::fibers::fiber::id fid;
            };
        }

        /**
         * \brief Class which controls terminating the program once the user has requested the program to stop
         * 
         * This converts the \c SIGINT signal (or an arbitrary source) into a \c spacepi::util::InterruptException which
         * will be thrown on all threads at the next cancellation point.
         * 
         * Cancellation points are locations at which it would be safe to throw the exception, and during long-running
         * operations which would need to be cancelled.
         * 
         * For applications which need to cancel out of a wait on a \c std::condition_variable, see
         * InterruptableConditionVariable.
         * Additionally, other methods thoughout the SpacePi core library are marked as cancellation points, and will
         * throw the exception if a cancellation is requested.
         * 
         * A cancellation exception will only be thrown once per thread, so once it is caught, it must be handled and
         * stop any long-running loops on that thread.
         */
        class Interrupt final {
            public:
                /**
                 * \brief The exception which is thrown when a cancellation is requested
                 */
                static const spacepi::util::InterruptException cancelException;

                /**
                 * \brief Call this function at various points in the code to allow the cancellation exception to be
                 * thrown.
                 * 
                 * This function will throw a \c spacepi::util::InterruptException in the event that a cancellation was
                 * requested by the user.
                 */
                static inline void cancellationPoint() {
                    if (instance.state == Cancelled) {
                        instance.throwCancelled();
                    }
                }

                /**
                 * \brief Cause a cancellation of this program
                 * 
                 * This will cause the next cancellation point which is run on each thread to throw an exception
                 */
                static void cancel() noexcept;

            private:
                enum State {
                    Running,
                    Cancelling,
                    Cancelled,
                    Stopping
                };

                static void interruptSignalHandler(int sig) noexcept;

                Interrupt() noexcept;
                ~Interrupt() noexcept;

                void throwCancelled();
                void cancellationThreadFunc() noexcept;

                static Interrupt instance;

                sig_atomic_t state;
                std::set<detail::ThreadID> dispatched;
                std::mutex mtx;
                std::condition_variable cond;
                std::thread thread;
        };
    }
}

#endif
