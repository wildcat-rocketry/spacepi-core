#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/CloneImageStep.hpp>
#include <spacepi/liblinux/InstallationData.hpp>

using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void CloneImageStep::run(InstallationData &data) {
    throw EXCEPTION(NotImplementedException("CloneImageStep not implemented"));
}
