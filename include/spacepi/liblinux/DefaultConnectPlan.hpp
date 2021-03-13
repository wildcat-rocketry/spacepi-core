#ifndef SPACEPI_TARGETLIB_LINUX_DEFAULTCONNECTPLAN_HPP
#define SPACEPI_TARGETLIB_LINUX_DEFAULTCONNECTPLAN_HPP

#include <spacepi/liblinux/InstallationPlan.hpp>

namespace spacepi {
    namespace liblinux {
        class DefaultConnectPlan : public InstallationPlan {
            public:
                DefaultConnectPlan() noexcept;
        };
    }
}

#endif
