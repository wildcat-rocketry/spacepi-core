#include <string>
#include <SpacePi.hpp>
#include <vector>
#include <spacepi/liblinux/steps/MountPartitionsStep.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/SharedTempDir.hpp>
#include <spacepi/liblinux/Image.hpp>
#include <spacepi/liblinux/SharedMount.hpp>

using namespace std;
using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void MountPartitionsStep::run(InstallationData &data) {
    data.initData<SharedTempDir>("mount");
    string root = data.getData<SharedTempDir>().getPath();
    Image &image = data.getData<Image>();
    PartitionTable &pt = data.getData<PartitionTable>();
    std::vector<SharedMount> mounts = image.mountPartitions(pt, root, true);
    data.initData<std::vector<SharedMount>>(mounts);
}
