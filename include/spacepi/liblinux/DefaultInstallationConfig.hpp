#ifndef SPACEPI_TARGETLIB_LINUX_DEFAULTINSTALLATIONCONFIG_HPP
#define SPACEPI_TARGETLIB_LINUX_DEFAULTINSTALLATIONCONFIG_HPP

#include <spacepi/liblinux/InstallationConfig.hpp>

namespace spacepi {
    namespace liblinux {
        class DefaultInstallationConfig : public InstallationConfig {
            public:
                DefaultInstallationConfig() noexcept;
        };
    }
}

#endif
