#ifndef SPACEPI_TARGETLIB_LINUX_STEPS_FORMATPARTITIONSSTEPSTEP_HPP
#define SPACEPI_TARGETLIB_LINUX_STEPS_FORMATPARTITIONSSTEPSTEP_HPP

#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationStep.hpp>

namespace spacepi {
    namespace liblinux {
        namespace steps {
            class FormatPartitionsStep : public InstallationStep {
                public:
                    void run(InstallationData &data);
            };
        }
    }
}

#endif
