#ifndef SPACEPI_TARGET_RPI_EXTENSION_PROCESSORFACTORY_HPP
#define SPACEPI_TARGET_RPI_EXTENSION_PROCESSORFACTORY_HPP

#include <memory>
#include <string>
#include <SpacePi.hpp>

namespace spacepi {
    namespace target {
        namespace extension {
            class ProcessorFactory : public spacepi::resource::ResourceFactory<spacepi::resource::Processor> {
                public:
                    static ProcessorFactory instance;

                    ProcessorFactory() noexcept;

                private:
                    std::shared_ptr<spacepi::resource::Processor> create(const std::string &name) noexcept;

                    std::weak_ptr<spacepi::resource::Processor> ptr;
            };
        }
    }
}

#endif
