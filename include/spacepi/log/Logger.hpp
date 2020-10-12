#ifndef SPACEPI_CORE_LOG_LOGGER_HPP
#define SPACEPI_CORE_LOG_LOGGER_HPP

#include <string>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/log/LogManager.hpp>
#include <spacepi/log/LogStream.hpp>

namespace spacepi {
    namespace log {
        class Logger {
            public:
                Logger(const std::string &tag, LogManager &manager = LogManager::instance);

                LogStream operator ()(const LogLevel &level) const;

            private:
                const std::string tag;
                LogManager &manager;
        };
    }
}

#endif
