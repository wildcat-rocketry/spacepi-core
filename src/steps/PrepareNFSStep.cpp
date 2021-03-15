#include <boost/filesystem.hpp>
#include <spacepi/liblinux/steps/PrepareNFSStep.hpp>
#include <spacepi/liblinux/InstallationConfig.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/SharedTempDir.hpp>

using namespace boost::filesystem;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void PrepareNFSStep::run(InstallationData &data) {
    const InstallationConfig &config = data.getData<InstallationConfig>();
    path root = data.getData<SharedTempDir>().getPath();
    boost::filesystem::rename(root / config.nfsDir, root / path(config.nfsDir).parent_path() / ("." + path(config.nfsDir).filename().native() + "~"));
    create_directory(root / config.nfsDir);
}
