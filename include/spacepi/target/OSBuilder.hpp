#ifndef SPACEPI_TARGET_RPI_OSBUILDER_HPP
#define SPACEPI_TARGET_RPI_OSBUILDER_HPP

#include <string>
#include <SpacePi.hpp>
#include <spacepi/liblinux/InstallationOptions.hpp>

namespace spacepi {
    namespace target {
        class OSBuilder : public spacepi::util::CommandConfigurable {
            public:
                OSBuilder(spacepi::util::Command &cmd);

                void runCommand();

            private:
                spacepi::liblinux::InstallationOptions opts;
        };
    }
}

#endif
