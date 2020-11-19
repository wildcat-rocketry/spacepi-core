#ifndef SPACEPI_CORE_LOG_ENTRY_HPP
#define SPACEPI_CORE_LOG_ENTRY_HPP

#include <chrono>
#include <string>
#include <spacepi/log/LogLevel.hpp>

namespace spacepi {
    namespace log {
        class Entry {
            public:
                Entry() noexcept;
                Entry(const std::string &tag, const LogLevel &level, const std::chrono::system_clock::time_point &time, const std::string &message) noexcept;

                const std::string &getTag() const noexcept;
                const LogLevel &getLevel() const noexcept;
                const std::chrono::system_clock::time_point &getTime() const noexcept;
                const std::string &getMessage() const noexcept;

            private:
                const std::string tag;
                const LogLevel &level;
                const std::chrono::system_clock::time_point time;
                const std::string message;
        };
    }
}

#endif
