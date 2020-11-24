#ifndef SPACEPI_CORE_UTIL_COMMANDCONFIGURABLE_HPP
#define SPACEPI_CORE_UTIL_COMMANDCONFIGURABLE_HPP

#include <memory>
#include <string>
#include <utility>
#include <spacepi/util/CommandInternals.hpp>

namespace spacepi {
    namespace util {
        class CommandConfigurable {
            friend class Command;

            protected:
                CommandConfigurable(const std::string &caption, Command &cmd) noexcept;

                template <typename Type>
                void fromCommand(Type &var, const std::string &name, const std::string &desc) {
                    group.add(std::move(std::shared_ptr<detail::GenericCommandParser>(new detail::CommandParser<Type>(var, name, desc))));
                }

                template <typename Type>
                void fromCommand(Type &var, const Type &def, const std::string &name, const std::string &desc) {
                    group.add(std::move(std::shared_ptr<detail::GenericCommandParser>(new detail::CommandParser<Type>(var, def, name, desc))));
                }

                // The compiler wants to call the previous prototype with a const char * instead of an std::string so add another overload
                template <typename Type>
                void fromCommand(Type &var, const std::string &def, const std::string &name, const std::string &desc) {
                    group.add(std::move(std::shared_ptr<detail::GenericCommandParser>(new detail::CommandParser<Type>(var, def, name, desc))));
                }

                virtual void runCommand() = 0;

            private:
                detail::CommandGroup &group;
        };
    }
}

#endif
