#ifndef SPACEPI_CORE_PACKAGE_USER_HPP
#define SPACEPI_CORE_PACKAGE_USER_HPP

#include <string>
#include <vector>

namespace spacepi {
    namespace package {
        /**
         * \brief Data class to represent a user account configuration
         */
        class User {
            public:
                /**
                 * \brief Gets the name of the person
                 * 
                 * \return The name
                 */
                const std::string &getName() const noexcept;

                /**
                 * \brief Sets the name of the person
                 * 
                 * \param[in] name The name
                 */
                void setName(const std::string &name) noexcept;

                /**
                 * \brief Gets the name of the user
                 * 
                 * \return The name
                 */
                const std::string &getUsername() const noexcept;

                /**
                 * \brief Sets the name of the user
                 * 
                 * \param[in] username The name
                 */
                void setUsername(const std::string &username) noexcept;

                /**
                 * \brief Gets the email account of the person
                 * 
                 * \return The email
                 */
                const std::string &getEmail() const noexcept;

                /**
                 * \brief Sets the email account of the person
                 * 
                 * \param[in] email The email
                 */
                void setEmail(const std::string &email) noexcept;

                /**
                 * \copydoc getKeys()
                 */
                const std::vector<std::string> &getKeys() const noexcept;

                /**
                 * \brief Gets a list of public SSH keys authorized to the user
                 * 
                 * \return The list of keys
                 */
                std::vector<std::string> &getKeys() noexcept;

            private:
                std::string name;
                std::string username;
                std::string email;
                std::vector<std::string> keys;
        };
    }
}

#endif
