#include <string>
#include <boost/filesystem.hpp>
#include <SpacePi.hpp>
#include <vector>
#include <spacepi/liblinux/steps/MountPartitionsStep.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/SharedTempDir.hpp>
#include <spacepi/liblinux/Image.hpp>
#include <spacepi/liblinux/SharedMount.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void MountPartitionsStep::run(InstallationData &data) {
    data.initData<SharedTempDir>("mount");
    string root = data.getData<SharedTempDir>().getPath();
    Image &image = data.getData<Image>();
    PartitionTable &pt = data.getData<PartitionTable>();
    std::vector<SharedMount> mounts = image.mountPartitions(pt, root, true);
    path procfs = root / "proc";
    create_directories(procfs);
    mounts.emplace_back("none", procfs.native(), "nosuid,nodev,noexec,noatime", "proc");
    path sysfs = root / "sys";
    create_directories(sysfs);
    mounts.emplace_back("none", sysfs.native(), "nosuid,nodev,noexec,noatime", "sysfs");
    data.initData<std::vector<SharedMount>>(mounts);
}
