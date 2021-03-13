#ifndef SPACEPI_TARGETLIB_LINUX_STEPS_CLONEIMAGESTEP_HPP
#define SPACEPI_TARGETLIB_LINUX_STEPS_CLONEIMAGESTEP_HPP

#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationStep.hpp>

namespace spacepi {
    namespace liblinux {
        namespace steps {
            class CloneImageStep : public InstallationStep {
                public:
                    void run(InstallationData &data);
            };
        }
    }
}

#endif
