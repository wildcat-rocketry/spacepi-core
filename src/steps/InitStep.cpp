#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/InitStep.hpp>
#include <spacepi/liblinux/InstallationData.hpp>

using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void InitStep::run(InstallationData &data) {
    throw EXCEPTION(StateException("Not implemented"));
}
