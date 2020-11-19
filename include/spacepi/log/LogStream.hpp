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
        class LogStream : public std::ostream {
            public:
                LogStream(const std::string &tag, const LogLevel &level, LogManager &manager) noexcept;
                LogStream(const LogStream &copy) noexcept;
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
