#ifndef SPACEPI_TARGETLIB_LINUX_INSTALLATIONSTEP_HPP
#define SPACEPI_TARGETLIB_LINUX_INSTALLATIONSTEP_HPP

#include <spacepi/liblinux/InstallationData.hpp>

namespace spacepi {
    namespace liblinux {
        class InstallationStep {
            public:
                virtual void run(InstallationData &data) = 0;
        };
    }
}

#endif
