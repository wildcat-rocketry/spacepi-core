#ifndef SPACEPI_CORE_LOG_LOGLEVEL_HPP
#define SPACEPI_CORE_LOG_LOGLEVEL_HPP

#include <ostream>
#include <string>

namespace spacepi {
    namespace log {
        class LogLevel {
            public:
                LogLevel(int id, const std::string &tag);

                bool operator ==(const LogLevel &other) const;
                bool operator !=(const LogLevel &other) const;
                bool operator < (const LogLevel &other) const;
                bool operator <=(const LogLevel &other) const;
                bool operator > (const LogLevel &other) const;
                bool operator >=(const LogLevel &other) const;

                const std::string &getTag() const;

                static const LogLevel Debug;
                static const LogLevel Info;
                static const LogLevel Warning;
                static const LogLevel Error;

            private:
                int id;
                std::string tag;
        };
    }
}

std::ostream &operator <<(std::ostream &os, const spacepi::log::LogLevel &level);

#endif
