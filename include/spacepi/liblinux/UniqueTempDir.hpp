#ifndef SPACEPI_TARGETLIB_LINUX_UNIQUETEMPDIR_HPP
#define SPACEPI_TARGETLIB_LINUX_UNIQUETEMPDIR_HPP

#include <string>

namespace spacepi {
    namespace liblinux {
        /**
         * \brief Provides a managed resource for temporary directories
         * 
         * This class is used to create temporary directories on the system that will be automatically deleted when the
         * code is finished using it.
         * The directory will always be in the \c /tmp directory, and the name will contain three identifiers: the word
         * \c spacepi, the name given to the constructor, and a randomized string.
         * 
         * Example use:
         * \code{.cpp}
         * UniqueTempDir dir("this-is-the-name");
         * dir.doesExist(); // true
         * dir.getPath(); // "/tmp/spacepi-this-is-the-name-abc123XX"
         * dir.mkdir(); // has no effect
         * dir.rmdir(); // deletes everything in /tmp/spacepi-this-is-the-name-abc123XX
         * dir.mkdir();
         * dir.getPath(); // "/tmp/spacepi-this-is-the-name-123abcYY"
         * \endcode
         */
        class UniqueTempDir {
            public:
                /**
                 * \brief Create a UniqueTempDir create a directory for this resource to represent
                 * 
                 * \param[in] name A string used to identify the temporary directory
                 * 
                 * \see mkdir()
                 */
                UniqueTempDir(const std::string &name);

                /**
                 * \brief Destroy the UniqueTempDir and make sure the temporary directory gets deleted if it still
                 * exists.
                 * 
                 * \see rmdir()
                 */
                ~UniqueTempDir();

                UniqueTempDir(UniqueTempDir &) = delete;
                UniqueTempDir &operator =(UniqueTempDir &) = delete;

                UniqueTempDir(UniqueTempDir &&move) noexcept;
                UniqueTempDir &operator =(UniqueTempDir &&move) noexcept;

                const std::string &getPath() const noexcept;

                bool doesExist() const noexcept;

                /**
                 * \brief Creates a new, randomized directory name which does not currently exist.
                 * 
                 * If this UniqueTempDir already represents a created directory, this function does nothing.
                 */
                void mkdir();

                /**
                 * \brief Deletes the directory and any contained files which this UniqueTempDir currently represents.
                 * 
                 * If this UniqueTempDir does not represent a created directory, this function does nothing.
                 */
                void rmdir();

            private:
                std::string name;
                std::string path;
                bool exists;
        };
    }
}

#endif
