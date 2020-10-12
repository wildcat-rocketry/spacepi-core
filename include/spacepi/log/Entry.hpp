#ifndef SPACEPI_CORE_LOG_ENTRY_HPP
#define SPACEPI_CORE_LOG_ENTRY_HPP

#include <chrono>
#include <string>
#include <spacepi/log/LogLevel.hpp>

namespace spacepi {
    namespace log {
        class Entry {
            public:
                Entry();
                Entry(const std::string &tag, const LogLevel &level, const std::chrono::system_clock::time_point &time, const std::string &message);

                const std::string &getTag() const;
                const LogLevel &getLevel() const;
                const std::chrono::system_clock::time_point &getTime() const;
                const std::string &getMessage() const;

            private:
                const std::string tag;
                const LogLevel &level;
                const std::chrono::system_clock::time_point time;
                const std::string message;
        };
    }
}

#endif
