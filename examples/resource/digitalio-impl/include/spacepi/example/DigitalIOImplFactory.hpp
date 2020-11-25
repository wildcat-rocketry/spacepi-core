#ifndef SPACEPI_EXAMPLE_DIGITALIOIMPLFACTORY_HPP
#define SPACEPI_EXAMPLE_DIGITALIOIMPLFACTORY_HPP

#include <memory>
#include <string>
#include <spacepi/resource/DigitalIO.hpp>
#include <spacepi/resource/ResourceFactory.hpp>

namespace spacepi {
    namespace example {
        class DigitalIOImplFactory : public spacepi::resource::ResourceFactory<spacepi::resource::DigitalIO> {
            public:
                static DigitalIOImplFactory instance;

                DigitalIOImplFactory();

            private:
                std::shared_ptr<spacepi::resource::DigitalIO> create(const std::string &name);
        };
    }
}

#endif
