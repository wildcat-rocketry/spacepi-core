#include <string>
#include <boost/filesystem.hpp>
#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/InstallSpacePiStep.hpp>
#include <spacepi/liblinux/Config.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationConfig.hpp>
#include <spacepi/liblinux/InstallationOptions.hpp>
#include <spacepi/liblinux/SharedTempDir.hpp>
#include <spacepi/liblinux/UniqueChroot.hpp>
#include <spacepi/liblinux/UniqueProcess.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void InstallSpacePiStep::run(InstallationData &data) {
    string rootDir = data.getData<SharedTempDir>().getPath();
    InstallationConfig &config = data.getData<InstallationConfig>();
    path configFile = relative(data.getData<InstallationOptions>().getConfigFile(), CMAKE_SOURCE_DIR);
    UniqueChroot chroot(rootDir);
    path buildDir = config.sourceDir / "build";
    UniqueProcess make(false, false, false, "/usr/bin/env", { "make", "-C", buildDir.native(), "install" });
    make.wait();
    if (make.getExitCode() != 0) {
        throw EXCEPTION(ResourceException("Unable to install SpacePi"));
    }
    UniqueProcess ldconfig(false, false, false, "/sbin/ldconfig", {});
    ldconfig.wait();
    if (ldconfig.getExitCode() != 0) {
        throw EXCEPTION(ResourceException("Unable to set library include path"));
    }
    UniqueProcess spacepictl(false, false, false, "/usr/local/bin/spacepictl", { "config-set", configFile.native() });
    spacepictl.wait();
    if (spacepictl.getExitCode() != 0) {
        throw EXCEPTION(ResourceException("Unable to set SpacePi package config"));
    }
}
