#ifndef SPACEPI_CORE_DTC_TOKENIZER_TOKEN_HPP
#define SPACEPI_CORE_DTC_TOKENIZER_TOKEN_HPP

#include <ostream>
#include <string>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>

namespace spacepi {
    namespace dtc {
        namespace tokenizer {
            /**
             * \brief Model class for a whitespace-delimited token in a source file
             *
             * A token is a continuous string, which is broken according to the following rules:
             * \li All C-style line and block comments are broken and discarded
             * \li Any string contained between two ""s are preserved as a single token (including the quotes)
             * \li The following characters are considered invalid (generate warning), then discarded: !$%'()*\^`|~
             * \li All ASCII characters greater than '~' are considered invalid (generate warning), then discarded
             * \li All ASCII characters less than or equal to ' ' (space) are broken and discarded
             * \li The following characters are broken into their own tokens, but not discarded: :;<>[]{}
             * \li Each remaining continuous block is treated as a token
             */
            class Token {
                public:
                    /**
                     * \brief Constructs a Token
                     *
                     * \param[in] location The SourceLocation of the Token
                     * \param[in] value The string value of the Token
                     */
                    Token(const diagnostics::SourceLocation &location, const std::string &value) noexcept;

                    /**
                     * \brief Checks for equality
                     *
                     * \param[in] other The other object
                     * \return The equality result
                     */
                    bool operator==(const Token &other) const noexcept;

                    /**
                     * \brief Checks for equality
                     *
                     * \param[in] other The other object
                     * \return The equality result
                     */
                    bool operator!=(const Token &other) const noexcept;

                    /**
                     * \brief Gets the SourceLocation of the Token
                     *
                     * \return The SourceLocation
                     */
                    const diagnostics::SourceLocation &getLocation() const noexcept;

                    /**
                     * \brief Gets the value of the Token
                     *
                     * \return The value
                     */
                    const std::string &getValue() const noexcept;

                private:
                    diagnostics::SourceLocation location;
                    std::string value;
            };

            /**
             * \brief Prints a Token to a formatted stream for debugging
             *
             * \param[in] os The stream to which to print
             * \param[in] obj The object to print
             * \return \c os
             */
            std::ostream &operator<<(std::ostream &os, const Token &obj) noexcept;
        }
    }
}

#endif
