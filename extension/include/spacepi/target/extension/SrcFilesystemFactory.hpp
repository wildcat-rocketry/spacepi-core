#ifndef SPACEPI_TARGETLIB_LINUX_EXTENSION_SRCFILESYSTEMFACTORY_HPP
#define SPACEPI_TARGETLIB_LINUX_EXTENSION_SRCFILESYSTEMFACTORY_HPP

#include <string>
#include <spacepi/target/extension/detail/FilesystemFactoryBase.hpp>

namespace spacepi {
    namespace target {
        namespace extension {
            class SrcFilesystemFactory : public detail::FilesystemFactoryBase {
                public:
                    static SrcFilesystemFactory instance;

                    SrcFilesystemFactory() noexcept;

                private:
                    std::string getPath(const std::string &relativePath) noexcept;
            };
        }
    }
}

#endif
