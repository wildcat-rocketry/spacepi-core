#include <spacepi/liblinux/DefaultInstallationConfig.hpp>

using namespace spacepi::liblinux;

DefaultInstallationConfig::DefaultInstallationConfig() noexcept {
    sourceDir = "/usr/local/src/spacepi";
    nfsDirs = { "/etc", "/usr/local", "/var/local" };
    packages =
        ",automake"
        ",build-essential"
        ",cmake"
        ",debootstrap"
        ",device-tree-compiler"
        ",gdb"
        ",git"
        ",gpiod"
        ",htop"
        ",i2c-tools"
        ",kpartx"
        ",libblkid-dev"
        ",libboost-all-dev"
        ",libgit2-dev"
        ",libgpiod-dev"
        ",libi2c-dev"
        ",libprotobuf-dev"
        ",libssl-dev"
        ",man-db"
        ",network-manager"
        ",nfs-kernel-server"
        ",openssh-server"
        ",protobuf-compiler"
        ",psmisc"
        ",rng-tools"
        ",screen"
        ",strace"
        ",sudo"
        ",tmux"
            + 1;
    components = "main,contrib,non-free";
    osRelease = "stable";
    aptURL = "http://deb.debian.org/debian";
    sourceUid = 0;
    sourceGid = 0;
}
