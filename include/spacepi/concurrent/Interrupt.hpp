#ifndef SPACEPI_CORE_CONCURRENT_INTERRUPT_HPP
#define SPACEPI_CORE_CONCURRENT_INTERRUPT_HPP

#include <condition_variable>
#include <csignal>
#include <mutex>
#include <set>
#include <thread>
#include <boost/fiber/all.hpp>

namespace spacepi {
    namespace concurrent {
        namespace detail {
            class ThreadID {
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
        class Interrupt {
            public:
                /**
                 * \brief Call this function at various points in the code to allow the cancellation exception to be
                 * thrown.
                 * 
                 * This function will throw a \c spacepi::util::InterruptException in the event that a cancellation was
                 * requested by the user.
                 */
                static inline void cancellationPoint() {
                    if (isCancelled) {
                        throwCancelled();
                    }
                }

                /**
                 * \brief Cause a cancellation of this program
                 * 
                 * This will cause the next cancellation point which is run on each thread to throw an exception
                 */
                static void cancel() noexcept;

                /**
                 * \brief Signal handler which handles the cancellation of this program
                 * 
                 * This signal handler is automatically installed on program start and should not need to be referenced
                 * anywhere outside of the core library.
                 * 
                 * \param[in] sig The signal number
                 */
                static void interruptSignalHandler(int sig) noexcept;

            private:
                static void throwCancelled();
                static void cancellationThreadFunc() noexcept;

                Interrupt() = default;

                static sig_atomic_t isCancelled;
                static std::set<detail::ThreadID> cancelDispatched;
                static std::mutex cancellationMutex;
                static std::condition_variable cancellationCond;
                static bool cancelling;
                static std::thread cancellationThread;
        };
    }
}

#endif
