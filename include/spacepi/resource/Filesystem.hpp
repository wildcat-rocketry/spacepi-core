#ifndef SPACEPI_CORE_RESOURCE_FILESYSTEM_HPP
#define SPACEPI_CORE_RESOURCE_FILESYSTEM_HPP

#include <memory>
#include <string>
#include <spacepi/resource/Stream.hpp>

namespace spacepi {
    namespace resource {
        /**
         * \brief A hardware resource which allows reading and writing files
         */
        class Filesystem {
            public:
                /**
                 * \brief Modes in which a file can be opened
                 */
                enum OpenMode {
                    /**
                     * \brief Open file for reading only
                     */
                    Read,
                    /**
                     * \brief Open file for writing and delete old contents
                     */
                    Write,
                    /**
                     * \brief Open file for writing and change filename if file already exists
                     */
                    Log
                };

                /**
                 * \brief Destroys this Filesystem resource
                 */
                virtual ~Filesystem() = default;

                /**
                 * \brief Get a pointer to a Filesystem object by its resource name
                 * 
                 * \param[in] name The resource name for which to get the Filesystem object
                 * 
                 * \return The Filesystem object, or a null pointer if the name is not registered
                 */
                static std::shared_ptr<Filesystem> get(const std::string &name);

                /**
                 * \brief Opens a file
                 * 
                 * \param[in] name The relative filename, or an empty string for the default
                 * \param[in] mode The mode to open the stream in
                 * 
                 * \return A stream to access the file
                 */
                virtual std::shared_ptr<Stream> open(const std::string &name = "", enum OpenMode mode = Read) = 0;
        };
    }
}

#endif
