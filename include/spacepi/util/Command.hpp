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

        class Command : private spacepi::log::AutoLog<decltype("core"_autolog)> {
            friend class CommandConfigurable;

            public:
                Command(int argc, const char **argv) noexcept;

                Command(Command &) = delete;
                Command &operator =(Command &) = delete;

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
