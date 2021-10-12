#ifndef SPACEPI_CORE_DTC_LEXER_LEXEME_HPP
#define SPACEPI_CORE_DTC_LEXER_LEXEME_HPP

#include <ostream>
#include <stdint.h>
#include <string>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>

namespace spacepi {
    namespace dtc {
        namespace lexer {
            /**
             * \brief Model class for a Token with some context
             */
            class Lexeme {
                public:
                    /**
                     * \brief The type of Lexeme
                     */
                    enum Type
                    {
                        /**
                         * \brief The tag describing the current version
                         *
                         * Matches: \c /dts-v1/
                         *
                         * After:
                         * \li (file start)
                         *
                         * Before:
                         * \li \c Semicolon
                         */
                        VersionTag,
                        /**
                         * \brief A semicolon
                         *
                         * Matches: \c ;
                         *
                         * After:
                         * \li \c VersionTag
                         * \li \c BraceClose
                         * \li \c AngleClose
                         * \li \c StringLiteral
                         * \li \c BracketClose
                         * \li \c LabelPath
                         *
                         * Before:
                         * \li (file end)
                         * \li \c RootNode
                         * \li \c BraceClose
                         * \li \c NodeName
                         * \li \c LabelName
                         * \li \c PropertyName
                         * \li \c IncludeTag
                         */
                        Semicolon,
                        /**
                         * \brief The root node name
                         *
                         * Matches: \c /
                         *
                         * After:
                         * \li \c Semicolon
                         *
                         * Before:
                         * \li \c BraceOpen
                         */
                        RootNode,
                        /**
                         * \brief A left curly brace
                         *
                         * Matches: \c {
                         *
                         * After:
                         * \li \c RootNode
                         * \li \c NodeName
                         *
                         * Before:
                         * \li \c BraceClose
                         * \li \c LabelName
                         * \li \c NodeName
                         * \li \c PropertyName
                         * \li \c IncludeTag
                         */
                        BraceOpen,
                        /**
                         * \brief A right curly brace
                         *
                         * Matches: \c }
                         *
                         * After:
                         * \li \c Semicolon
                         * \li \c BraceOpen
                         *
                         * Before:
                         * \li \c Semicolon
                         */
                        BraceClose,
                        /**
                         * \brief The name of a node label
                         *
                         * Matches: \c [a-zA-Z,._+-][a-zA-Z0-9,._+-]{0,30}
                         *
                         * After:
                         * \li \c Semicolon
                         * \li \c BraceOpen
                         *
                         * Before:
                         * \li \c Colon
                         */
                        LabelName,
                        /**
                         * \brief A colon
                         *
                         * Matches: \c :
                         *
                         * After:
                         * \li \c LabelName
                         *
                         * Before:
                         * \li \c NodeName
                         */
                        Colon,
                        /**
                         * \brief The name of a node
                         *
                         * Matches: \c [a-zA-Z,._+-][a-zA-Z0-9,._+-]{0,30}(@[0-9]{1,8})?
                         *
                         * After:
                         * \li \c Semicolon
                         * \li \c BraceOpen
                         * \li \c Colon
                         *
                         * Before:
                         * \li \c BraceOpen
                         */
                        NodeName,
                        /**
                         * \brief The name of a property
                         *
                         * Matches: \c [#?a-zA-Z,._+-][#?a-zA-Z0-9,._+-]{0,30}
                         *
                         * After:
                         * \li \c Semicolon
                         * \li \c BraceOpen
                         *
                         * Before:
                         * \li \c Equals
                         */
                        PropertyName,
                        /**
                         * \brief An equals sign
                         *
                         * Matches: \c =
                         *
                         * After:
                         * \li \c PropertyName
                         *
                         * Before:
                         * \li \c AngleOpen
                         * \li \c StringLiteral
                         * \li \c BracketOpen
                         * \li \c LabelPath
                         */
                        Equals,
                        /**
                         * \brief A less than sign
                         *
                         * Matches: \c <
                         *
                         * After:
                         * \li \c Equals
                         *
                         * Before:
                         * \li \c AngleClose
                         * \li \c IntLiteral
                         * \li \c LabelReference
                         */
                        AngleOpen,
                        /**
                         * \brief A greater than sign
                         *
                         * Matches: \c >
                         *
                         * After:
                         * \li \c AngleOpen
                         * \li \c IntLiteral
                         * \li \c LabelReference
                         *
                         * Before:
                         * \li \c Semicolon
                         */
                        AngleClose,
                        /**
                         * \brief A 32-bit numeric literal
                         *
                         * Matches: \c 0x[0-9a-fA-F]{1,8}|[0-9]{1,10}
                         *
                         * After:
                         * \li \c AngleOpen
                         * \li \c IntLiteral
                         * \li \c LabelReference
                         *
                         * Before:
                         * \li \c AngleClose
                         * \li \c IntLiteral
                         * \li \c LabelReference
                         */
                        IntLiteral,
                        /**
                         * \brief A reference to a label's 32-bit phandle
                         *
                         * Matches: \c &[a-zA-Z,._+-][a-zA-Z0-9,._+-]{0,30}(@[0-9]{1,8})?
                         *
                         * After:
                         * \li \c AngleOpen
                         * \li \c IntLiteral
                         * \li \c LabelReference
                         *
                         * Before:
                         * \li \c AngleClose
                         * \li \c IntLiteral
                         * \li \c LabelReference
                         */
                        LabelReference,
                        /**
                         * \brief A string literal property value
                         *
                         * Matches: \c "[^"]*"
                         *
                         * After:
                         * \li \c Equals
                         * \li \c IncludeTag
                         *
                         * Before:
                         * \li \c Semicolon
                         */
                        StringLiteral,
                        /**
                         * \brief A left bracket
                         *
                         * Matches: \c \[
                         *
                         * After:
                         * \li \c Equals
                         *
                         * Before:
                         * \li \c BracketClose
                         * \li \c ByteLiteral
                         */
                        BracketOpen,
                        /**
                         * \brief A right bracket
                         *
                         * Matches: \c \]
                         *
                         * After:
                         * \li \c BracketOpen
                         * \li \c ByteLiteral
                         *
                         * Before:
                         * \li \c Semicolon
                         */
                        BracketClose,
                        /**
                         * \brief An 8-bit numeric literal
                         *
                         * Matches: \c [0-9a-fA-F]{1,2}
                         *
                         * After:
                         * \li \c BracketOpen
                         * \li \c ByteLiteral
                         *
                         * Before:
                         * \li \c BracketClose
                         * \li \c ByteLiteral
                         */
                        ByteLiteral,
                        /**
                         * \brief A reference to a label's path
                         *
                         * Matches: \c &[a-zA-Z,._+-][a-zA-Z0-9,._+-]{0,30}(@[0-9]{1,8})?
                         *
                         * After:
                         * \li \c Equals
                         *
                         * Before:
                         * \li \c Semicolon
                         */
                        LabelPath,
                        /**
                         * \brief The keyword declaring an include
                         *
                         * Matches: \c /include/
                         *
                         * After:
                         * \li \c BraceOpen
                         * \li \c Semicolon
                         *
                         * Before:
                         * \li \c StringLiteral
                         */
                        IncludeTag
                    };

                    /**
                     * \brief Constructs a Lexeme
                     *
                     * \param[in] type The type of the Lexeme
                     * \param[in] location The SourceLocation of the Lexeme
                     */
                    Lexeme(Type type, const diagnostics::SourceLocation &location) noexcept;

                    /**
                     * \brief Constructs a Lexeme
                     *
                     * \param[in] type The type of the Lexeme
                     * \param[in] location The SourceLocation of the Lexeme
                     * \param[in] value The integer value of the Lexeme
                     */
                    Lexeme(Type type, const diagnostics::SourceLocation &location, uint32_t value) noexcept;

                    /**
                     * \brief Constructs a Lexeme
                     *
                     * \param[in] type The type of the Lexeme
                     * \param[in] location The SourceLocation of the Lexeme
                     * \param[in] value The string value of the Lexeme
                     */
                    Lexeme(Type type, const diagnostics::SourceLocation &location, const std::string &value) noexcept;

                    /**
                     * \brief Checks for equality
                     *
                     * \param[in] other The other object
                     * \return The equality result
                     */
                    bool operator==(const Lexeme &other) const noexcept;

                    /**
                     * \brief Checks for equality
                     *
                     * \param[in] other The other object
                     * \return The equality result
                     */
                    bool operator!=(const Lexeme &other) const noexcept;

                    /**
                     * \brief Gets the SourceLocation of the Lexeme
                     *
                     * \return The SourceLocation
                     */
                    const diagnostics::SourceLocation &getLocation() const noexcept;

                    /**
                     * \brief Gets the type of the Lexeme
                     *
                     * \return The type
                     */
                    Type getType() const noexcept;

                    /**
                     * \brief Gets the integer value of the Lexeme
                     *
                     * \return The value
                     */
                    uint32_t getIntegerValue() const noexcept;

                    /**
                     * \brief Gets the string value of the Lexeme
                     *
                     * \return The value
                     */
                    const std::string &getStringValue() const noexcept;

                private:
                    diagnostics::SourceLocation location;
                    Type type;
                    uint32_t intVal;
                    std::string strVal;
            };

            /**
             * \brief Prints a Type to a formatted stream for debugging
             *
             * \param[in] os The stream to which to print
             * \param[in] obj The object to print
             * \return \c os
             */
            std::ostream &operator<<(std::ostream &os, Lexeme::Type obj) noexcept;

            /**
             * \brief Prints a Lexeme to a formatted stream for debugging
             *
             * \param[in] os The stream to which to print
             * \param[in] obj The object to print
             * \return \c os
             */
            std::ostream &operator<<(std::ostream &os, const Lexeme &obj) noexcept;
        }
    }
}

#endif
