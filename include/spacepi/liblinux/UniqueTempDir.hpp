#ifndef SPACEPI_TARGETLIB_LINUX_UNIQUETEMPDIR_HPP
#define SPACEPI_TARGETLIB_LINUX_UNIQUETEMPDIR_HPP

#include <string>

namespace spacepi {
    namespace liblinux {
        class UniqueTempDir {
            public:
                UniqueTempDir(const std::string &name);
                ~UniqueTempDir();

                UniqueTempDir(UniqueTempDir &) = delete;
                UniqueTempDir &operator =(UniqueTempDir &) = delete;

                UniqueTempDir(UniqueTempDir &&) noexcept = default;
                UniqueTempDir &operator =(UniqueTempDir &&) noexcept = default;

                const std::string &getPath() const noexcept;

                bool doesExist() const noexcept;
                void mkdir();
                void rmdir();

            private:
                std::string path;
                bool exists;
        };
    }
}

#endif
