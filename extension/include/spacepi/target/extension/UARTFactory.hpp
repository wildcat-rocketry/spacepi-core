#ifndef SPACEPI_TARGET_RPI_EXTENSION_UARTFACTORY_HPP
#define SPACEPI_TARGET_RPI_EXTENSION_UARTFACTORY_HPP

#include <memory>
#include <string>
#include <SpacePi.hpp>

namespace spacepi {
    namespace target {
        namespace extension {
            class UARTFactory : public spacepi::resource::ResourceFactory<spacepi::resource::UART> {
                public:
                    static UARTFactory instance;

                    UARTFactory() noexcept;

                private:
                    std::shared_ptr<spacepi::resource::UART> create(const std::string &name);
            };
        }
    }
}

#endif
