#include <spacepi/liblinux/DefaultConnectPlan.hpp>
#include <spacepi/liblinux/DefaultInstallationPlan.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationOptions.hpp>
#include <spacepi/liblinux/InstallationPlan.hpp>
#include <spacepi/liblinux/Installer.hpp>

using namespace spacepi::liblinux;

Installer::Installer() noexcept : installPlan(DefaultInstallationPlan()), connectPlan(DefaultConnectPlan()) {
}

InstallationPlan &Installer::getInstallPlan() noexcept {
    return installPlan;
}

const InstallationPlan &Installer::getInstallPlan() const noexcept {
    return installPlan;
}

InstallationPlan &Installer::getConnectPlan() noexcept {
    return connectPlan;
}

const InstallationPlan &Installer::getConnectPlan() const noexcept {
    return connectPlan;
}

void Installer::install(const InstallationData &data) {
    if (data.getData<InstallationOptions>().getConnectSSH().empty()) {
        getInstallPlan().install(data);
    } else {
        getConnectPlan().install(data);
    }
}
