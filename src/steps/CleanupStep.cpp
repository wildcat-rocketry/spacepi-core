#include <vector>
#include <unistd.h>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/SharedLoopDevice.hpp>
#include <spacepi/liblinux/SharedMount.hpp>
#include <spacepi/liblinux/SharedTempDir.hpp>
#include <spacepi/liblinux/steps/CleanupStep.hpp>

using namespace std;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void CleanupStep::run(InstallationData &data) {
    sync();
    data.deleteData<vector<SharedMount>>();
    data.deleteData<SharedLoopDevice>();
    data.deleteData<SharedTempDir>();
}
