#include <spacepi/liblinux/DefaultInstallationConfig.hpp>

using namespace spacepi::liblinux;

DefaultInstallationConfig::DefaultInstallationConfig() noexcept {
    sourceDir = "/usr/local/src/spacepi";
    packages = "cmake,build-essential,protobuf-compiler,libboost-all-dev";
    components = "main,contrib,non-free";
    osRelease = "stable";
    aptURL = "http://deb.debian.org/debian";
}
