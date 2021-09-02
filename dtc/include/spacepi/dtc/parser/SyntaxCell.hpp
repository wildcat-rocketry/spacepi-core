#ifndef SPACEPI_CORE_DTC_PARSER_SYNTAXCELL_HPP
#define SPACEPI_CORE_DTC_PARSER_SYNTAXCELL_HPP

#include <ostream>
#include <stdint.h>
#include <string>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>

namespace spacepi {
    namespace dtc {
        namespace parser {
            /**
             * \brief Model class for a 32-bit cell value
             */
            class SyntaxCell {
                public:
                    /**
                     * \brief Constructs a SyntaxCell
                     *
                     * \param[in] location The SourceLocation of this cell
                     * \param[in] value The literal value of the cell
                     */
                    SyntaxCell(const diagnostics::SourceLocation &location, uint32_t value) noexcept;

                    /**
                     * \brief Constructs a SyntaxCell
                     *
                     * \param[in] location The SourceLocation of this cell
                     * \param[in] label The label for the phandle reference
                     */
                    SyntaxCell(const diagnostics::SourceLocation &location, const std::string &label) noexcept;

                    /**
                     * \brief Checks for equality
                     *
                     * \param[in] other The other object
                     * \return The equality result
                     */
                    bool operator==(const SyntaxCell &other) const noexcept;

                    /**
                     * \brief Checks for equality
                     *
                     * \param[in] other The other object
                     * \return The equality result
                     */
                    bool operator!=(const SyntaxCell &other) const noexcept;

                    /**
                     * \brief Gets the SourceLocation of this cell
                     *
                     * \return The SourceLocation
                     */
                    const diagnostics::SourceLocation &getLocation() const noexcept;

                    /**
                     * \brief Gets the literal value of the cell
                     *
                     * This is only valid if \c isLabel() \c == \c false
                     *
                     * \return The value
                     */
                    uint32_t getValue() const noexcept;

                    /**
                     * \brief Gets the label for the phandle reference
                     *
                     * This is only valid if \c isLabel() \c == \c true
                     *
                     * \return The label
                     */
                    const std::string &getLabel() const noexcept;

                    /**
                     * \brief Determines if this SyntaxCell has a label for a phandle reference, not a literal value
                     *
                     * \return If it does
                     */
                    bool isLabel() const noexcept;

                private:
                    diagnostics::SourceLocation location;
                    uint32_t value;
                    std::string label;
            };

            /**
             * \brief Prints a SyntaxCell to a formatted stream for debugging
             *
             * \param[in] os The stream to which to print
             * \param[in] obj The object to print
             * \return \c os
             */
            std::ostream &operator<<(std::ostream &os, const SyntaxCell &obj) noexcept;
        }
    }
}

#endif
