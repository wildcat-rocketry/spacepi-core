#include <spacepi/liblinux/DefaultInstallationConfig.hpp>

using namespace spacepi::liblinux;

DefaultInstallationConfig::DefaultInstallationConfig() noexcept {
    sourceDir = "/usr/local/src/spacepi";
    packages =
        ",automake"
        ",build-essential"
        ",cmake"
        ",debootstrap"
        ",git"
        ",kpartx"
        ",libblkid-dev"
        ",libboost-all-dev"
        ",libgit2-dev"
        ",libgpiod-dev"
        ",libi2c-dev"
        ",libprotobuf-dev"
        ",libssl-dev"
        ",protobuf-compiler"
        ",sudo"
            + 1;
    components = "main,contrib,non-free";
    osRelease = "stable";
    aptURL = "http://deb.debian.org/debian";
    sourceUid = 0;
    sourceGid = 0;
}
