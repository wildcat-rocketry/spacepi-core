#ifndef SPACEPI_CORE_LOG_LOGLEVEL_HPP
#define SPACEPI_CORE_LOG_LOGLEVEL_HPP

#include <ostream>
#include <string>

namespace spacepi {
    namespace log {
        class LogLevel {
            public:
                LogLevel(int id, const std::string &tag) noexcept;

                bool operator ==(const LogLevel &other) const noexcept;
                bool operator !=(const LogLevel &other) const noexcept;
                bool operator < (const LogLevel &other) const noexcept;
                bool operator <=(const LogLevel &other) const noexcept;
                bool operator > (const LogLevel &other) const noexcept;
                bool operator >=(const LogLevel &other) const noexcept;

                const std::string &getTag() const noexcept;

                static const LogLevel Debug;
                static const LogLevel Info;
                static const LogLevel Warning;
                static const LogLevel Error;

            private:
                const int id;
                const std::string tag;
        };
    }
}

std::ostream &operator <<(std::ostream &os, const spacepi::log::LogLevel &level);

#endif
