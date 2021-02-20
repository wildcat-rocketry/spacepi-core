#ifndef SPACEPI_CORE_LOG_LOGSTREAM_HPP
#define SPACEPI_CORE_LOG_LOGSTREAM_HPP

#include <chrono>
#include <ostream>
#include <sstream>
#include <string>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/log/LogManager.hpp>

namespace spacepi {
    namespace log {
        /**
         * \brief A stream which can be used to create an entry in the log
         * 
         * It is preferred to use the Logger class instead of this class directly.
         */
        class LogStream final : public std::ostream {
            public:
                /**
                 * \brief Create a new LogStream
                 * 
                 * \param[in] tag The tag of the Logger
                 * \param[in] level The level at which the Entry will be created
                 * \param[in] manager The logging manager to log to
                 */
                LogStream(const std::string &tag, const LogLevel &level, LogManager &manager) noexcept;

                /**
                 * \brief Copy a LogStream
                 * 
                 * \param[in] copy The LogStream to copy
                 */
                LogStream(const LogStream &copy) noexcept;

                /**
                 * \brief Destroy a LogStream
                 * 
                 * When this destructor is called, the entry is automatically pushed to the LogManager
                 */
                ~LogStream();

                LogStream &operator =(const LogStream &) = delete;

            private:
                const std::string tag;
                const LogLevel &level;
                LogManager &manager;
                const std::chrono::system_clock::time_point time;
                std::stringbuf buffer;
        };
    }
}

#endif
