#ifndef SPACEPI_TARGETLIB_LINUX_STEPS_CLEANUPSTEP_HPP
#define SPACEPI_TARGETLIB_LINUX_STEPS_CLEANUPSTEP_HPP

#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationStep.hpp>

namespace spacepi {
    namespace liblinux {
        namespace steps {
            class CleanupStep : public InstallationStep {
                public:
                    void run(InstallationData &data);
            };
        }
    }
}

#endif
