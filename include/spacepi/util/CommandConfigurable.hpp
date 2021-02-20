#ifndef SPACEPI_CORE_UTIL_COMMANDCONFIGURABLE_HPP
#define SPACEPI_CORE_UTIL_COMMANDCONFIGURABLE_HPP

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <spacepi/util/CommandInternals.hpp>

namespace spacepi {
    namespace util {
        /**
         * \brief Base class for classes which represent functionality that needs to be configured from a Command
         */
        class CommandConfigurable {
            friend class Command;

            protected:
                /**
                 * \brief Initialize a CommandConfigurable
                 * 
                 * \param[in] caption A label under which all options for this CommandConfigurable will be listed when
                 * printing help text
                 * \param[in,out] cmd The Command from which this CommandConfigurable should be configured
                 */
                CommandConfigurable(const std::string &caption, Command &cmd) noexcept;

                /**
                 * \brief Destroys this CommandConfigurable
                 */
                virtual ~CommandConfigurable() = default;

                /**
                 * \brief Initialize a variable from the Command's command-line arguments
                 * 
                 * Note: this method does not set \c var, but \c var will be set before runCommand() is called
                 * 
                 * \param[out] var The variable to read from the command-line arguments
                 * \param[in] name The name of the option to use to determine the variable value
                 * \param[in] desc A short description of what the option does
                 */
                template <typename Type>
                void fromCommand(Type &var, const std::string &name, const std::string &desc) {
                    group.add(std::move(std::shared_ptr<detail::GenericCommandParser>(new detail::CommandParser<Type>(var, name, desc))));
                }

                /**
                 * \brief Initialize a variable from the Command's command-line arguments
                 * 
                 * Note: this method does not set \c var, but \c var will be set before runCommand() is called
                 * 
                 * \param[out] var The variable to read from the command-line arguments
                 * \param[in] def The default value to use in case no argument for this option is provided
                 * \param[in] name The name of the option to use to determine the variable value
                 * \param[in] desc A short description of what the option does
                 */
                template <typename Type>
                void fromCommand(Type &var, const Type &def, const std::string &name, const std::string &desc) {
                    group.add(std::move(std::shared_ptr<detail::GenericCommandParser>(new detail::CommandParser<Type>(var, def, name, desc))));
                }

                /**
                 * \brief Initialize a variable from the Command's command-line arguments
                 * 
                 * Note: this method does not set \c var, but \c var will be set before runCommand() is called
                 * 
                 * The compiler wants to call the previous prototype with a \c const \c char \c * instead of an \c std::string, so this overload was added.
                 * 
                 * \param[out] var The variable to read from the command-line arguments
                 * \param[in] def The default value to use in case no argument for this option is provided
                 * \param[in] name The name of the option to use to determine the variable value
                 * \param[in] desc A short description of what the option does
                 */
                void fromCommand(std::string &var, const std::string &def, const std::string &name, const std::string &desc) {
                    group.add(std::move(std::shared_ptr<detail::GenericCommandParser>(new detail::CommandParser<std::string>(var, def, name, desc))));
                }

                /**
                 * \brief Runs any actions associated with this CommandConfigurable
                 * 
                 * By the time this method is called, all variables given to fromCommand() will be initialized to their proper values
                 */
                virtual void runCommand() = 0;

            private:
                detail::CommandGroup &group;
        };
    }
}

#endif
