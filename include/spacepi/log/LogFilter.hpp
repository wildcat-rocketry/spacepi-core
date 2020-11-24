#ifndef SPACEPI_CORE_LOG_LOGFILTER_HPP
#define SPACEPI_CORE_LOG_LOGFILTER_HPP

#include <string>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/util/Trie.hpp>

namespace spacepi {
    namespace log {
        class LogFilter {
            public:
                LogFilter() noexcept;

                bool operator ()(const std::string &name, const LogLevel &level) const noexcept;

                const LogLevel &getDefaultLevel() const noexcept;
                void setDefaultLevel(const LogLevel &level) noexcept;

                const LogLevel &getLevel(const std::string &name) const noexcept;
                void setLevel(const std::string &name, const LogLevel &level) noexcept;
                void resetLevel(const std::string &name) noexcept;

            private:
                const LogLevel *defaultLevel;
                spacepi::util::Trie<const LogLevel *> levels;
        };
    }
}

#endif
