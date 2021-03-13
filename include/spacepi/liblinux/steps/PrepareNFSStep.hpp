#ifndef SPACEPI_TARGETLIB_LINUX_STEPS_PREPARENFSSTEP_HPP
#define SPACEPI_TARGETLIB_LINUX_STEPS_PREPARENFSSTEP_HPP

#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationStep.hpp>

namespace spacepi {
    namespace liblinux {
        namespace steps {
            class PrepareNFSStep : public InstallationStep {
                public:
                    void run(InstallationData &data);
            };
        }
    }
}

#endif
