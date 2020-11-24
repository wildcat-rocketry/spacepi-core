#ifndef SPACEPI_CORE_LOG_LOGFILTERCOMMAND_HPP
#define SPACEPI_CORE_LOG_LOGFILTERCOMMAND_HPP

#include <string>
#include <vector>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

namespace spacepi {
    namespace util {
        class Command;
    }

    namespace log {
        class LogFilterCommand : private spacepi::util::CommandConfigurable {
            public:
                LogFilterCommand(spacepi::util::Command &cmd) noexcept;

                void runCommand();

            private:
                const LogLevel *defaultLevel;
                std::vector<std::string> debugLevel;
                std::vector<std::string> infoLevel;
                std::vector<std::string> warningLevel;
                std::vector<std::string> errorLevel;
        };
    }
}

#endif
