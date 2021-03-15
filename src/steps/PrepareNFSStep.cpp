#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <spacepi/liblinux/steps/PrepareNFSStep.hpp>
#include <spacepi/liblinux/InstallationConfig.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/SharedTempDir.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void PrepareNFSStep::run(InstallationData &data) {
    const InstallationConfig &config = data.getData<InstallationConfig>();
    path root = data.getData<SharedTempDir>().getPath();
    for (vector<string>::const_iterator it = config.nfsDirs.begin(); it != config.nfsDirs.end(); ++it) {
        boost::filesystem::rename(root / *it, root / path(*it).parent_path() / ("." + path(*it).filename().native() + "~"));
        create_directory(root / *it);
    }
}
