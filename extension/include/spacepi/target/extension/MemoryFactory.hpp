#ifndef SPACEPI_TARGETLIB_LINUX_EXTENSION_MEMORYFACTORY_HPP
#define SPACEPI_TARGETLIB_LINUX_EXTENSION_MEMORYFACTORY_HPP

#include <memory>
#include <string>
#include <SpacePi.hpp>

namespace spacepi {
    namespace target {
        namespace extension {
            class MemoryFactory : public spacepi::resource::ResourceFactory<spacepi::resource::Memory> {
                public:
                    static MemoryFactory instance;

                    MemoryFactory() noexcept;

                private:
                    std::shared_ptr<spacepi::resource::Memory> create(const std::string &name) noexcept;

                    std::weak_ptr<spacepi::resource::Memory> ptr;
            };
        }
    }
}

#endif
