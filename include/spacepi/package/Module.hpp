#ifndef SPACEPI_CORE_PACKAGE_MODULE_HPP
#define SPACEPI_CORE_PACKAGE_MODULE_HPP

#include <string>
#include <spacepi/package/Options.hpp>

namespace spacepi {
    namespace package {
        /**
         * \brief Data class to represent a module configuration
         */
        class Module final {
            public:
                /**
                 * \brief Gets the name of the module instance
                 * 
                 * \return The name
                 */
                const std::string &getName() const noexcept;

                /**
                 * \brief Sets the name of the module instance
                 * 
                 * \param[in] name The new name
                 */
                void setName(const std::string &name) noexcept;

                /**
                 * \brief Gets the type of the module
                 * 
                 * \return The type
                 */
                const std::string &getType() const noexcept;

                /**
                 * \brief Sets the type of the module
                 * 
                 * \param[in] type The type
                 */
                void setType(const std::string &type) noexcept;

                /**
                 * \brief Determines if the module is automatically started on system boot
                 * 
                 * \return If it is
                 */
                bool hasAutomaticStart() const noexcept;

                /**
                 * \brief Sets whether or not the module is automatically started on system boot
                 * 
                 * \param[in] automaticStart If it is
                 */
                void setAutomaticStart(bool automaticStart) noexcept;

                /**
                 * \copydoc getOptions()
                 */
                const Options &getOptions() const noexcept;

                /**
                 * \brief Gets the options used as parameters to the module
                 * 
                 * \return The options
                 */
                Options &getOptions() noexcept;

            private:
                std::string name;
                std::string type;
                bool automaticStart;
                Options options;
        };
    }
}

#endif
