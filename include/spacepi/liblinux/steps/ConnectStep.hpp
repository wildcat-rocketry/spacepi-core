#ifndef SPACEPI_TARGETLIB_LINUX_STEPS_CONNECTSTEP_HPP
#define SPACEPI_TARGETLIB_LINUX_STEPS_CONNECTSTEP_HPP

#include <SpacePi.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationStep.hpp>
#include <spacepi/liblinux/SystemCaller.hpp>

namespace spacepi {
    namespace liblinux {
        namespace steps {
            class ConnectStep : public InstallationStep, private SystemCaller, private spacepi::log::AutoLog<decltype("spacepi:liblinux:connect"_autolog)> {
                public:
                    void run(InstallationData &data);
            };
        }
    }
}

#endif
