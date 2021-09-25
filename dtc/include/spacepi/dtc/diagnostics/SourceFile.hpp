#ifndef SPACEPI_CORE_DTC_DIAGNOSTICS_SOURCEFILE_HPP
#define SPACEPI_CORE_DTC_DIAGNOSTICS_SOURCEFILE_HPP

#include <ostream>
#include <string>
#include <vector>

namespace spacepi {
    namespace dtc {
        namespace diagnostics {
            /**
             * \brief Model class for a file containing device tree source code
             */
            class SourceFile {
                public:
                    /**
                     * \brief Constructs a SourceFile
                     *
                     * \param[in] filename The path to the SourceFile
                     * \param[in] lines The lines of code in the SourceFile
                     */
                    SourceFile(const std::string &filename, const std::vector<std::string> &lines) noexcept;

                    /**
                     * \brief Checks for equality
                     *
                     * \param[in] other The other object
                     * \return The equality result
                     */
                    bool operator==(const SourceFile &other) const noexcept;

                    /**
                     * \brief Checks for equality
                     *
                     * \param[in] other The other object
                     * \return The equality result
                     */
                    bool operator!=(const SourceFile &other) const noexcept;

                    /**
                     * \brief Gets the path to the SourceFile
                     *
                     * \return The path
                     */
                    const std::string &getFilename() const noexcept;

                    /**
                     * \brief Gets the lines of code in the SourceFile
                     *
                     * \return The lines
                     */
                    const std::vector<std::string> &getLines() const noexcept;

                private:
                    std::string filename;
                    std::vector<std::string> lines;
            };

            /**
             * \brief Prints a SourceFile to a formatted stream for debugging
             *
             * \param[in] os The stream to which to print
             * \param[in] obj The object to print
             * \return \c os
             */
            std::ostream &operator<<(std::ostream &os, const SourceFile &obj) noexcept;
        }
    }
}

#endif
