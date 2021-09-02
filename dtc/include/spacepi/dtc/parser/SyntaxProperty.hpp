#ifndef SPACEPI_CORE_DTC_PARSER_SYNTAXPROPERTY_HPP
#define SPACEPI_CORE_DTC_PARSER_SYNTAXPROPERTY_HPP

#include <ostream>
#include <stdint.h>
#include <string>
#include <vector>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>
#include <spacepi/dtc/parser/SyntaxCell.hpp>

namespace spacepi {
    namespace dtc {
        namespace parser {
            /**
             * \brief Model class for a node property
             */
            class SyntaxProperty {
                public:
                    /**
                     * \brief Constructs a SyntaxProperty
                     *
                     * \param[in] location The SourceLocation of this property
                     * \param[in] name The name of the property
                     * \param[in] cells The cells which make up the value of the property
                     */
                    SyntaxProperty(const diagnostics::SourceLocation &location, const std::string &name, const std::vector<SyntaxCell> &cells) noexcept;

                    /**
                     * \brief Constructs a SyntaxProperty
                     *
                     * \param[in] location The SourceLocation of this property
                     * \param[in] name The name of the property
                     * \param[in] bytes The raw value of the property
                     */
                    SyntaxProperty(const diagnostics::SourceLocation &location, const std::string &name, const std::vector<uint8_t> &bytes) noexcept;

                    /**
                     * \brief Constructs a SyntaxProperty
                     *
                     * \param[in] location The SourceLocation of this property
                     * \param[in] name The name of the property
                     * \param[in] label The label to a node whose path should become the property's value
                     */
                    SyntaxProperty(const diagnostics::SourceLocation &location, const std::string &name, const std::string &label) noexcept;

                    /**
                     * \brief Gets the raw value of a string's bytes
                     *
                     * \param[in] str The string value
                     * \return The raw value
                     */
                    static std::vector<uint8_t> literalStringBytes(const std::string &str) noexcept;

                    /**
                     * \brief Checks for equality
                     *
                     * \param[in] other The other object
                     * \return The equality result
                     */
                    bool operator==(const SyntaxProperty &other) const noexcept;

                    /**
                     * \brief Checks for equality
                     *
                     * \param[in] other The other object
                     * \return The equality result
                     */
                    bool operator!=(const SyntaxProperty &other) const noexcept;

                    /**
                     * \brief Gets the SourceLocation of this property
                     *
                     * \return The SourceLocation
                     */
                    const diagnostics::SourceLocation &getLocation() const noexcept;

                    /**
                     * \brief Gets the name of the property
                     *
                     * \return The name
                     */
                    const std::string &getName() const noexcept;

                    /**
                     * \brief Gets the cells which make up the value of the property
                     *
                     * This is only valid if \c isString() \c == \c false
                     *
                     * \return The cells
                     */
                    const std::vector<SyntaxCell> &getCells() const noexcept;

                    /**
                     * \brief Gets the raw value of the property
                     *
                     * This is only valid if \c isLabel() \c == \c false
                     *
                     * \return The value
                     */
                    const std::vector<uint8_t> &getBytes() const noexcept;

                    /**
                     * \brief Gets the label to a node whose path should become the property's value
                     *
                     * This is only valid if \c isLabel() \c == \c true
                     *
                     * \return The label
                     */
                    const std::string &getLabel() const noexcept;

                    /**
                     * \brief Determines if this property has a string value, not a cell value
                     *
                     * \return If it does
                     */
                    bool isString() const noexcept;

                    /**
                     * \brief Determines if this property's string value is determined from a label
                     *
                     * This is only valid if \c isString() \c == \c true
                     *
                     * \return If it is
                     */
                    bool isLabel() const noexcept;

                private:
                    diagnostics::SourceLocation location;
                    std::string name;
                    std::vector<SyntaxCell> cells;
                    std::vector<uint8_t> bytes;
                    std::string label;
            };

            /**
             * \brief Prints a SyntaxProperty to a formatted stream for debugging
             *
             * \param[in] os The stream to which to print
             * \param[in] obj The object to print
             * \return \c os
             */
            std::ostream &operator<<(std::ostream &os, const SyntaxProperty &obj) noexcept;
        }
    }
}

#endif
