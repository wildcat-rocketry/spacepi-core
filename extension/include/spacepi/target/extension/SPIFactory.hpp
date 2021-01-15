#ifndef SPACEPI_TARGET_RPI_EXTENSION_SPIFACTORY_HPP
#define SPACEPI_TARGET_RPI_EXTENSION_SPIFACTORY_HPP

#include <memory>
#include <string>
#include <SpacePi.hpp>

namespace spacepi {
    namespace target {
        namespace extension {
            class SPIFactory : public spacepi::resource::ResourceFactory<spacepi::resource::SPI> {
                public:
                    static SPIFactory instance;

                    SPIFactory() noexcept;

                private:
                    std::shared_ptr<spacepi::resource::SPI> create(const std::string &name);
            };
        }
    }
}

#endif
