#ifndef SPACEPI_TARGET_RPI_DOWNLOADFIRMWARESTEP_HPP
#define SPACEPI_TARGET_RPI_DOWNLOADFIRMWARESTEP_HPP

#include <SpacePi.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationStep.hpp>

namespace spacepi {
    namespace target {
        class DownloadFirmwareStep : public spacepi::liblinux::InstallationStep, private spacepi::log::AutoLog<decltype("spacepi:rpi"_autolog)> {
            public:
                void run(spacepi::liblinux::InstallationData &data);
        };
    }
}

#endif
