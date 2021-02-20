#ifndef SPACEPI_CORE_LOG_LOGGER_HPP
#define SPACEPI_CORE_LOG_LOGGER_HPP

#include <string>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/log/LogManager.hpp>
#include <spacepi/log/LogStream.hpp>

namespace spacepi {
    namespace log {
        /**
         * \brief The main public interface used to create logging entries
         * 
         * These may be created either manually, or with AutoLog if used inside a class.
         * 
         * \code{.cpp}
         * Logger log("myLogger");
         * log(LogLevel::Info) << "This is an informative message.";
         * log(LogLevel::Error) << "This is an error message\nthat takes up multiple lines.";
         * log(LogLevel::Debug) << "This is 2+2: " << (2 + 2) << "!";
         * \endcode
         */
        class Logger final {
            public:
                /**
                 * \brief Initialize a new Logger
                 * 
                 * \param[in] tag The tag used to identify the Logger
                 * \param[in,out] manager The LogManager which this Logger should log to
                 */
                Logger(const std::string &tag, LogManager &manager = LogManager::instance) noexcept;

                /**
                 * \brief Creates a new LogStream which is valid for a single logging entry
                 * 
                 * Once the LogStream is deconstructed, the entry will be logged to the LogManager
                 * 
                 * \param[in] level The level at which to create the logging entry
                 * 
                 * \return The stream to log to
                 */
                LogStream operator ()(const LogLevel &level) const noexcept;

            private:
                const std::string tag;
                LogManager &manager;
        };
    }
}

#endif
