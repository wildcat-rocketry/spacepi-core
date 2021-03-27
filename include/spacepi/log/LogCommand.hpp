#ifndef SPACEPI_CORE_LOG_LOGCOMMAND_HPP
#define SPACEPI_CORE_LOG_LOGCOMMAND_HPP

#include <string>
#include <vector>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/resource/Filesystem.hpp>
#include <spacepi/resource/ResourcePtr.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

namespace spacepi {
    namespace util {
        class Command;
    }

    namespace log {
        /**
         * \brief A command which is used to configure the logging system based on command-line arguments
         */
        class LogCommand final : private spacepi::util::CommandConfigurable {
            public:
                /**
                 * \brief Initializes a new LogCommand
                 * 
                 * \param[in,out] cmd The spacepi::util::Command from which the logging system will be configured
                 */
                LogCommand(spacepi::util::Command &cmd) noexcept;

                void runCommand();

            private:
                const LogLevel *defaultLevel;
                std::vector<std::string> debugLevel;
                std::vector<std::string> infoLevel;
                std::vector<std::string> warningLevel;
                std::vector<std::string> errorLevel;
                spacepi::resource::ResourcePtr<spacepi::resource::Filesystem> logFile;
        };
    }
}

#endif
