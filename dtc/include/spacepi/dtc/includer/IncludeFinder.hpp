#ifndef SPACEPI_CORE_DTC_INCLUDER_INCLUDEFILEFINDER_HPP
#define SPACEPI_CORE_DTC_INCLUDER_INCLUDEFILEFINDER_HPP

#include <memory>
#include <string>
#include <vector>

namespace spacepi {
    namespace dtc {
        namespace includer {
            /**
             * \brief Base class for the include file locator
             *
             * This class is responsible for locating files to include from the path
             */
            class IncludeFinder {
                public:
                    /**
                     * \brief Destroys an IncludeFileFinder
                     */
                    virtual ~IncludeFinder() noexcept = default;

                    /**
                     * \brief Locates a file in the path of include directories. An empty string if not found
                     *
                     * \param[in] filename The name of the file (relative to an includeDir) to locate
                     * \param[in] includeDirs The directories in which source files may reside
                     * \return The full path to the file
                     */
                    virtual const std::string &locate(const std::string &filename, const std::vector<std::string> &includeDirs) const noexcept = 0;

                    /**
                     * \brief The instance of the IncludeFileFinder
                     */
                    static std::shared_ptr<IncludeFinder> instance;
            };
        }
    }
}

#endif
