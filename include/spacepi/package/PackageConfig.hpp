#ifndef SPACEPI_CORE_PACKAGE_PACKAGECONFIG_HPP
#define SPACEPI_CORE_PACKAGE_PACKAGECONFIG_HPP

#include <istream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <spacepi/package/Module.hpp>
#include <spacepi/package/Options.hpp>
#include <spacepi/package/User.hpp>

namespace spacepi {
    namespace package {
        /**
         * \brief Data class to represent a package configuration
         */
        class PackageConfig {
            public:
                /**
                 * \brief Create an empty package configuration
                 */
                PackageConfig() noexcept = default;

                /**
                 * \brief Read a package configuration from an XML file
                 * 
                 * \param[in] filename The file to read
                 */
                PackageConfig(const std::string &filename);

                /**
                 * \brief Loads the package configuration from an XML file
                 * 
                 * \param[in] filename The file to read
                 */
                void load(const std::string &filename);

                /**
                 * \brief Writes the package configuration to a stream
                 * 
                 * \param[in,out] os The stream to write to
                 */
                void save(std::ostream &os);

                /**
                 * \brief Writes the package configuration to an XML file
                 * 
                 * \param[in] filename The name of the file to write
                 */
                void save(const std::string &filename);

                /**
                 * \brief Resets the PackageConfig to the initial values
                 */
                void reset() noexcept;

                /**
                 * \brief Gets the type of target for the configuration
                 * 
                 * \return The type of target
                 */
                const std::string &getTargetType() const noexcept;

                /**
                 * \brief Sets the type of target for the configuration
                 * 
                 * \param[in] targetType The type of target
                 */
                void setTargetType(const std::string &targetType) noexcept;

                /**
                 * \brief Gets the name of the target for the configuration
                 * 
                 * \return The name of the target
                 */
                const std::string &getTargetName() const noexcept;

                /**
                 * \brief Sets the name of the target for the configuration
                 * 
                 * \param[in] targetName The name of the target
                 */
                void setTargetName(const std::string &targetName) noexcept;

                /**
                 * \copydoc getTargetOptions()
                 */
                const Options &getTargetOptions() const noexcept;

                /**
                 * \brief Gets the options associated with the target
                 * 
                 * \return The options
                 */
                Options &getTargetOptions() noexcept;

                /**
                 * \copydoc getUsers()
                 */
                const std::unordered_map<std::string, User> &getUsers() const noexcept;

                /**
                 * \brief Gets a list of users to configure for the package
                 * 
                 * \return The users
                 */
                std::unordered_map<std::string, User> &getUsers() noexcept;

                /**
                 * \copydoc getModules()
                 */
                const std::unordered_map<std::string, Module> &getModules() const noexcept;

                /**
                 * \brief Gets a list of modules to configure for the package
                 * 
                 * \return The modules
                 */
                std::unordered_map<std::string, Module> &getModules() noexcept;

            private:
                std::string targetType;
                std::string targetName;
                Options targetOptions;
                std::unordered_map<std::string, User> users;
                std::unordered_map<std::string, Module> modules;
        };
    }
}

#endif
