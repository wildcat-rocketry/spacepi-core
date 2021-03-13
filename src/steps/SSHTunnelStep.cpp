#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/SSHTunnelStep.hpp>
#include <spacepi/liblinux/InstallationData.hpp>

using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void SSHTunnelStep::run(InstallationData &data) {
    throw EXCEPTION(NotImplementedException("SSHTunnelStep not implemented"));
}
