#ifndef SPACEPI_TARGETLIB_LINUX_EXTENSION_VARFILESYSTEMFACTORY_HPP
#define SPACEPI_TARGETLIB_LINUX_EXTENSION_VARFILESYSTEMFACTORY_HPP

#include <string>
#include <spacepi/target/extension/detail/FilesystemFactoryBase.hpp>

namespace spacepi {
    namespace target {
        namespace extension {
            class VarFilesystemFactory : public detail::FilesystemFactoryBase {
                public:
                    static VarFilesystemFactory instance;

                    VarFilesystemFactory() noexcept;

                private:
                    std::string getPath(const std::string &relativePath) noexcept;
            };
        }
    }
}

#endif
