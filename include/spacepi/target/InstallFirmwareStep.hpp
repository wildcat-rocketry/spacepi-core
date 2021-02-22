#ifndef SPACEPI_TARGET_RPI_INSTALLFIRMWARESTEP_HPP
#define SPACEPI_TARGET_RPI_INSTALLFIRMWARESTEP_HPP

#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationStep.hpp>

namespace spacepi {
    namespace target {
        class InstallFirmwareStep : public spacepi::liblinux::InstallationStep {
            public:
                void run(spacepi::liblinux::InstallationData &data);
        };
    }
}

#endif
