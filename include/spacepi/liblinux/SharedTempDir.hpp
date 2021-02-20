#ifndef SPACEPI_TARGETLIB_LINUX_SHAREDTEMPDIR_HPP
#define SPACEPI_TARGETLIB_LINUX_SHAREDTEMPDIR_HPP

#include <memory>
#include <string>
#include <spacepi/liblinux/UniqueTempDir.hpp>

namespace spacepi {
    namespace liblinux {
        class SharedTempDir {
            public:
                SharedTempDir(const std::string &name);
                SharedTempDir(UniqueTempDir &&dir);
                ~SharedTempDir() noexcept(false);

                SharedTempDir(const SharedTempDir &) noexcept = default;
                SharedTempDir &operator =(const SharedTempDir &) noexcept = default;

                const std::string &getPath() const noexcept;

                bool doesExist() const noexcept;
                void mkdir();
                void rmdir();

            private:
                std::shared_ptr<UniqueTempDir> unique;
        };
    }
}

#endif
