#ifndef SPACEPI_CORE_LOG_LOGMANAGER_HPP
#define SPACEPI_CORE_LOG_LOGMANAGER_HPP

#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <spacepi/concurrent/RWMutex.hpp>
#include <spacepi/log/Entry.hpp>
#include <spacepi/log/LogFilter.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/log/LogTarget.hpp>
#include <spacepi/util/TemporalQueue.hpp>

namespace spacepi {
    namespace log {
        /**
         * \brief The top-level class of the logging package
         */
        class LogManager final {
            public:
                /**
                 * \brief The default, global LogManager instance
                 */
                static LogManager instance;

                /**
                 * \brief Initializes the LogManager
                 */
                LogManager();

                /**
                 * \brief Destroys the LogManager
                 */
                ~LogManager();

                LogManager(LogManager &) = delete;
                LogManager &operator =(LogManager &) = delete;

                /**
                 * \brief Writes a new Entry to any LogTargets that the LogFilter allows it to be sent to
                 * 
                 * \param[in] entry The logging Entry to write
                 */
                void operator <<(const Entry &entry);

                /**
                 * \brief Registers a new LogTarget with the LogManager
                 * 
                 * \param[in] target The new LogTarget to send messages to
                 * 
                 * \return \c this
                 */
                LogManager &operator +=(const std::shared_ptr<LogTarget> &target);

                /**
                 * \brief Gets the global logging filter for this LogManager
                 * 
                 * \return The LogFilter
                 */
                LogFilter &getFilter() noexcept;

                /**
                 * \copydoc getFilter()
                 */
                const LogFilter &getFilter() const noexcept;

                /**
                 * \brief Blocks until all log messages have been written to the console
                 */
                void flush() noexcept;

            private:
                void run();

                LogFilter filter;
                std::condition_variable cond;
                std::mutex entryMutex;
                spacepi::util::TemporalQueue<Entry, 64> entries;
                std::vector<std::shared_ptr<LogTarget>> targets;
                spacepi::concurrent::RWMutex<std::mutex, std::unique_lock<std::mutex>, std::condition_variable> targetLock;
                bool running;
                std::thread thread;
        };
    }
}

#endif
