#include <string>
#include <sys/sysinfo.h>
#include <boost/filesystem.hpp>
#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/BuildSpacePiStep.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationConfig.hpp>
#include <spacepi/liblinux/SharedTempDir.hpp>
#include <spacepi/liblinux/UniqueChroot.hpp>
#include <spacepi/liblinux/UniqueEID.hpp>
#include <spacepi/liblinux/UniqueMount.hpp>
#include <spacepi/liblinux/UniqueProcess.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void BuildSpacePiStep::run(InstallationData &data) {
    string rootDir = data.getData<SharedTempDir>().getPath();
    InstallationConfig &config = data.getData<InstallationConfig>();
    path procfs = rootDir / "proc";
    create_directories(procfs);
    UniqueMount procfsMount("none", procfs.native(), "nosuid,nodev,noexec,noatime", "proc");
    path sysfs = rootDir / "sys";
    create_directories(sysfs);
    UniqueMount sysfsMount("none", sysfs.native(), "nosuid,nodev,noexec,noatime", "sysfs");
    UniqueChroot chroot(rootDir);
    UniqueEID eid(config.sourceUid, config.sourceGid);
    path buildDir = config.sourceDir / "build";
    create_directories(buildDir);
    UniqueProcess cmake(false, false, false, "/usr/bin/env", { "cmake", "-DBUILD_MINIMAL=ON", "-S", config.sourceDir, "-B", buildDir.native() });
    cmake.wait();
    if (cmake.getExitCode() != 0) {
        throw EXCEPTION(ResourceException("Unable to configure SpacePi"));
    }
    int maxThreads = get_nprocs_conf();
    float maxLoad = ((float) maxThreads) * 0.75f;
    if (maxThreads < 1) {
        maxThreads = 1;
    }
    if (maxLoad < 1.f) {
        maxLoad = 1.f;
    }
    UniqueProcess make(false, false, false, "/usr/bin/env", { "make", "-C", buildDir.native(), "-j", to_string(maxThreads), "-l", to_string(maxLoad) });
    make.wait();
    if (make.getExitCode() != 0) {
        throw EXCEPTION(ResourceException("Unable to build SpacePi"));
    }
}
