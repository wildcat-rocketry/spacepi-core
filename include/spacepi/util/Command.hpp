#ifndef SPACEPI_CORE_UTIL_COMMAND_HPP
#define SPACEPI_CORE_UTIL_COMMAND_HPP

#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/log/LogFilterCommand.hpp>
#include <spacepi/util/CommandConfigurable.hpp>
#include <spacepi/util/CommandInternals.hpp>

namespace spacepi {
    namespace util {
        class Command;

        namespace detail {
            class CommandHelpWrapper : private CommandConfigurable {
                friend class spacepi::util::Command;

                public:
                    CommandHelpWrapper(Command &cmd) noexcept;

                    void runCommand();

                private:
                    bool showHelp;
            };
        }

        /**
         * \brief Representation of command-line arguments which can be used to configure a set of CommandConfigurable's
         */
        class Command : private spacepi::log::AutoLog<decltype("core"_autolog)> {
            friend class CommandConfigurable;

            public:
                /**
                 * \brief Initialize the Command
                 * 
                 * \param[in] argc The total number of command-line arguments to the program
                 * \param[in] argv The values of the command-line arguments to the program
                 */
                Command(int argc, const char **argv) noexcept;

                Command(Command &) = delete;
                Command &operator =(Command &) = delete;

                /**
                 * \brief Runs the CommandConfigurable actions that have been configured with this Command
                 */
                bool run() noexcept;

            private:
                std::vector<std::string> args;
                std::vector<detail::CommandGroup> groups;
                detail::CommandHelpWrapper helpWrapper;
                spacepi::log::LogFilterCommand logFilter;
        };
    }
}

#endif
