#ifndef SPACEPI_TARGETLIB_LINUX_INSTALLER_HPP
#define SPACEPI_TARGETLIB_LINUX_INSTALLER_HPP

#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationPlan.hpp>

namespace spacepi {
    namespace liblinux {
        class Installer {
            public:
                Installer() noexcept;

                InstallationPlan &getInstallPlan() noexcept;
                const InstallationPlan &getInstallPlan() const noexcept;

                InstallationPlan &getConnectPlan() noexcept;
                const InstallationPlan &getConnectPlan() const noexcept;

                void install(const InstallationData &data);

            private:
                InstallationPlan installPlan;
                InstallationPlan connectPlan;
        };
    }
}

#endif
