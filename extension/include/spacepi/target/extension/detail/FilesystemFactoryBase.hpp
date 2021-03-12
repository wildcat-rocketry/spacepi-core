#ifndef SPACEPI_TARGETLIB_LINUX_EXTENSION_DETAIL_FILESYSTEMFACTORYBASE_HPP
#define SPACEPI_TARGETLIB_LINUX_EXTENSION_DETAIL_FILESYSTEMFACTORYBASE_HPP

#include <memory>
#include <string>
#include <SpacePi.hpp>

namespace spacepi {
    namespace target {
        namespace extension {
            namespace detail {
                class FilesystemFactoryBase : public spacepi::resource::ResourceFactory<spacepi::resource::Filesystem> {
                    protected:
                        FilesystemFactoryBase(const std::string &name) noexcept;

                    private:
                        std::shared_ptr<spacepi::resource::Filesystem> create(const std::string &name);
                        virtual std::string getPath(const std::string &relativePath) noexcept = 0;

                        static std::string modulePath;
                };
            }
        }
    }
}

#endif
