#include <string>
#include <boost/filesystem.hpp>
#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/BuildSpacePiStep.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationConfig.hpp>
#include <spacepi/liblinux/SharedTempDir.hpp>
#include <spacepi/liblinux/UniqueChroot.hpp>
#include <spacepi/liblinux/UniqueEID.hpp>
#include <spacepi/liblinux/UniqueProcess.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void BuildSpacePiStep::run(InstallationData &data) {
    string rootDir = data.getData<SharedTempDir>().getPath();
    InstallationConfig &config = data.getData<InstallationConfig>();
    UniqueChroot chroot(rootDir);
    UniqueEID eid(config.sourceUid, config.sourceGid);
    path buildDir = config.sourceDir / "build";
    create_directories(buildDir);
    UniqueProcess cmake(false, false, false, "/usr/bin/env", { "cmake", "-DBUILD_MINIMAL=ON", "-S", config.sourceDir, "-B", buildDir.native() });
    cmake.wait();
    if (cmake.getExitCode() != 0) {
        throw EXCEPTION(ResourceException("Unable to configure SpacePi"));
    }
    UniqueProcess make(false, false, false, "/usr/bin/env", { "make", "-C", buildDir.native() });
    make.wait();
    if (make.getExitCode() != 0) {
        throw EXCEPTION(ResourceException("Unable to build SpacePi"));
    }
}
