#ifndef SPACEPI_TARGETLIB_LINUX_DEFAULTINSTALLATIONPLAN_HPP
#define SPACEPI_TARGETLIB_LINUX_DEFAULTINSTALLATIONPLAN_HPP

#include <spacepi/liblinux/InstallationPlan.hpp>

namespace spacepi {
    namespace liblinux {
        class DefaultInstallationPlan : public InstallationPlan {
            public:
                DefaultInstallationPlan() noexcept;
        };
    }
}

#endif
