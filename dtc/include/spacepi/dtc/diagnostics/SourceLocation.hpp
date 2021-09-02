#ifndef SPACEPI_CORE_DTC_DIAGNOSTICS_SOURCELOCATION_HPP
#define SPACEPI_CORE_DTC_DIAGNOSTICS_SOURCELOCATION_HPP

#include <limits>
#include <memory>
#include <ostream>
#include <spacepi/dtc/diagnostics/SourceFile.hpp>

namespace spacepi {
    namespace dtc {
        namespace diagnostics {
            /**
             * \brief Model class for a location within a SourceFile
             */
            class SourceLocation {
                public:
                    /**
                     * \brief Constructs an unknown SourceLocation
                     */
                    SourceLocation() noexcept = default;

                    /**
                     * \brief Constructs a SourceLocation
                     *
                     * \param[in] file The file in which this SourceLocation exists
                     * \param[in] lineNo The line number (starting at 1) of the SourceLocation
                     * \param[in] fromCol The first (inclusive) character index (starting at 1 at the start of the line)
                     * of the SourceLocation
                     * \param[in] toCol The last (inclusive) character index (starting at 1 at the start of the line) of
                     * the SourceLocation
                     */
                    SourceLocation(const std::shared_ptr<SourceFile> &file, int lineNo, int fromCol, int toCol) noexcept;

                    /**
                     * \brief Checks for equality
                     *
                     * \param[in] other The other object
                     * \return The equality result
                     */
                    bool operator==(const SourceLocation &other) const noexcept;

                    /**
                     * \brief Checks for equality
                     *
                     * \param[in] other The other object
                     * \return The equality result
                     */
                    bool operator!=(const SourceLocation &other) const noexcept;

                    /**
                     * \brief Determines if the SourceLocation represents a known location
                     *
                     * \return If it does
                     */
                    operator bool() const noexcept;

                    /**
                     * \brief Gets the file in which this SourceLocation exists
                     *
                     * \return The file
                     */
                    const std::shared_ptr<SourceFile> &getFile() const noexcept;

                    /**
                     * \brief Gets the line number (starting at 1) of the SourceLocation
                     *
                     * \return The line number
                     */
                    int getLineNumber() const noexcept;

                    /**
                     * \brief Gets the first (inclusive) character index (starting at 1 at the start of the line) of the
                     * SourceLocation
                     *
                     * \return The index
                     */
                    int getColumnStart() const noexcept;

                    /**
                     * \brief Gets the last (inclusive) character index (starting at 1 at the start of the line) of the
                     * SourceLocation
                     *
                     * \return The index
                     */
                    int getColumnEnd() const noexcept;

                private:
                    std::shared_ptr<SourceFile> file;
                    int lineNo;
                    int fromCol;
                    int toCol;
            };

            /**
             * \brief Prints a SourceLocation to a formatted stream for debugging
             *
             * \param[in] os The stream to which to print
             * \param[in] obj The object to print
             * \return \c os
             */
            std::ostream &operator<<(std::ostream &os, const SourceLocation &obj) noexcept;
        }
    }
}

#endif
