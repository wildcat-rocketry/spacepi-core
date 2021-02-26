#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/InstallBaseSystemStep.hpp>
#include <spacepi/liblinux/Config.hpp>
#include <spacepi/liblinux/InstallationConfig.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/SharedTempDir.hpp>
#include <spacepi/liblinux/UniqueChroot.hpp>
#include <spacepi/liblinux/UniqueProcess.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void InstallBaseSystemStep::run(InstallationData &data) {
    InstallationConfig &cfg = data.getData<InstallationConfig>();
    SharedTempDir &root = data.getData<SharedTempDir>();
    path cacheDir = read_symlink("/proc/self/exe").parent_path() / ".debootstrap-cache";
    create_directories(cacheDir);
    bool installedStage1 = false;
    for (int tries = 0; tries < 3; ++tries) {
        UniqueProcess debootstrapStage1(false, false, false, DEBOOTSTRAP_EXECUTABLE, {
            "--foreign",
            "--arch", cfg.dpkgArch,
            "--include", cfg.packages,
            "--components", cfg.components,
            "--cache-dir", cacheDir.native(),
            "--no-merged-usr",
            cfg.osRelease, root.getPath(), cfg.aptURL
        });
        debootstrapStage1.wait();
        if (debootstrapStage1.getExitCode() == 0) {
            installedStage1 = true;
            break;
        }
    }
    if (!installedStage1) {
        throw EXCEPTION(ResourceException("Failed to install base system (stage 1)."));
    }
    for (vector<string>::const_iterator it = cfg.emulationBin.begin(); it != cfg.emulationBin.end(); ++it) {
        path src = *it;
        path dst = root.getPath() / src;
        create_directories(dst.parent_path());
        copy(src, dst);
    }
    UniqueChroot chroot(root.getPath());
    UniqueProcess debootstrapStage2(false, false, false, "/debootstrap/debootstrap", { "--second-stage" });
    chroot.escape();
    debootstrapStage2.wait();
    if (debootstrapStage2.getExitCode() != 0) {
        throw EXCEPTION(ResourceException("Failed to install base system (stage 2)."));
    }
}
