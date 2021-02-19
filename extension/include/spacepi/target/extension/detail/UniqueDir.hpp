#ifndef SPACEPI_TARGETLIB_LINUX_EXTENSION_DETAIL_UNIQUEDIR_HPP
#define SPACEPI_TARGETLIB_LINUX_EXTENSION_DETAIL_UNIQUEDIR_HPP

#include <dirent.h>

namespace spacepi {
    namespace target {
        namespace extension {
            namespace detail {
                class UniqueDir {
                    public:
                        UniqueDir(DIR *dir = nullptr) noexcept;
                        ~UniqueDir() noexcept;

                        UniqueDir(UniqueDir &) = delete;
                        UniqueDir &operator =(UniqueDir &) = delete;

                        UniqueDir(UniqueDir &&move) noexcept;
                        UniqueDir &operator =(UniqueDir &&move) noexcept;

                        UniqueDir &operator =(DIR *dir) noexcept;
                        operator DIR *() const noexcept;
                        operator bool() const noexcept;

                        void release() noexcept;
                        void reset(DIR *dir = nullptr) noexcept;

                    private:
                        DIR *dir;
                };
            }
        }
    }
}

#endif
