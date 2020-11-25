#ifndef SPACEPI_EXAMPLE_APP_HPP
#define SPACEPI_EXAMPLE_APP_HPP

#include <spacepi/resource/DigitalIO.hpp>
#include <spacepi/resource/ResourcePtr.hpp>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

namespace spacepi {
    namespace example {
        class App : public spacepi::util::CommandConfigurable {
            public:
                App(spacepi::util::Command &cmd) noexcept;

                void runCommand();

            private:
                spacepi::resource::ResourcePtr<spacepi::resource::DigitalIO> pin;
        };
    }
}

#endif
