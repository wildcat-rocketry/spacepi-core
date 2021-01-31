#ifndef SPACEPI_TARGETLIB_LINUX_UNIQUETEMPDIR_HPP
#define SPACEPI_TARGETLIB_LINUX_UNIQUETEMPDIR_HPP

#include <spacepi/liblinux/SystemCaller.hpp>
#include <string>

namespace spacepi {
    namespace liblinux {
        class UniqueTempDir : private SystemCaller {
            public:
                UniqueTempDir(const std::string &name);
                ~UniqueTempDir();

                UniqueTempDir(UniqueTempDir &) = delete;
                UniqueTempDir &operator =(UniqueTempDir &) = delete;

                UniqueTempDir(UniqueTempDir &&move) noexcept;
                UniqueTempDir &operator =(UniqueTempDir &&move) noexcept;

                const std::string &getPath() const noexcept;

                bool doesExist() const noexcept;
                void mkdir();

                void rmdir();

            private:
                std::string name;
                std::string path;
                bool exists;
                void makePath(const std::string &name);
                std::string randomString(size_t length);
        };
    }
}

#endif
