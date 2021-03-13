#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/ConnectStep.hpp>
#include <spacepi/liblinux/InstallationData.hpp>

using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void ConnectStep::run(InstallationData &data) {
    throw EXCEPTION(NotImplementedException("ConnectStep not implemented"));
}
