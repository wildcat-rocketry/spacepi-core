#include <spacepi/liblinux/Image.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationOptions.hpp>
#include <spacepi/liblinux/steps/InitStep.hpp>

using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void InitStep::run(InstallationData &data) {
    InstallationOptions &installData = data.getData<InstallationOptions>();
    data.initData<Image>(installData.getOutFile());
}
