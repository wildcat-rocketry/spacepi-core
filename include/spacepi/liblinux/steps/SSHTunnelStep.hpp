#ifndef SPACEPI_TARGETLIB_LINUX_STEPS_SSHTUNNELSTEP_HPP
#define SPACEPI_TARGETLIB_LINUX_STEPS_SSHTUNNELSTEP_HPP

#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationStep.hpp>

namespace spacepi {
    namespace liblinux {
        namespace steps {
            class SSHTunnelStep : public InstallationStep {
                public:
                    void run(InstallationData &data);
            };
        }
    }
}

#endif
