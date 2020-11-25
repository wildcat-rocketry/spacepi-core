#ifndef SPACEPI_TARGET_RPI_EXTENSION_I2CFACTORY_HPP
#define SPACEPI_TARGET_RPI_EXTENSION_I2CFACTORY_HPP

#include <memory>
#include <string>
#include <SpacePi.hpp>

namespace spacepi {
    namespace target {
        namespace extension {
            class I2CFactory : public spacepi::resource::ResourceFactory<spacepi::resource::I2C> {
                public:
                    static I2CFactory instance;

                    I2CFactory() noexcept;

                private:
                    std::shared_ptr<spacepi::resource::I2C> create(const std::string &name);
            };
        }
    }
}

#endif
