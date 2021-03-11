#include <spacepi/liblinux/DefaultInstallationConfig.hpp>

using namespace spacepi::liblinux;

DefaultInstallationConfig::DefaultInstallationConfig() noexcept {
    sourceDir = "/usr/local/src/spacepi";
    packages =
        ",automake"
        ",build-essential"
        ",cmake"
        ",debootstrap"
        ",gdb"
        ",git"
        ",gpiod"
        ",i2c-tools"
        ",kpartx"
        ",libblkid-dev"
        ",libboost-all-dev"
        ",libgit2-dev"
        ",libgpiod-dev"
        ",libi2c-dev"
        ",libprotobuf-dev"
        ",libssl-dev"
        ",network-manager"
        ",openssh-server"
        ",protobuf-compiler"
        ",rng-tools"
        ",sudo"
            + 1;
    components = "main,contrib,non-free";
    osRelease = "stable";
    aptURL = "http://deb.debian.org/debian";
    sourceUid = 0;
    sourceGid = 0;
}
