#ifndef SPACEPI_TARGETLIB_LINUX_STEPS_ENSUREROOTSTEP_HPP
#define SPACEPI_TARGETLIB_LINUX_STEPS_ENSUREROOTSTEP_HPP

#include <SpacePi.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationStep.hpp>

namespace spacepi {
    namespace liblinux {
        namespace steps {
            class EnsureRootStep : public InstallationStep, private spacepi::log::AutoLog<decltype("spacepi:liblinux:permissions"_autolog)> {
                public:
                    void run(InstallationData &data);
            };
        }
    }
}

#endif
