#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/CleanupStep.hpp>
#include <spacepi/liblinux/InstallationData.hpp>

using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void CleanupStep::run(InstallationData &data) {
    throw EXCEPTION(StateException("Not implemented"));
}
