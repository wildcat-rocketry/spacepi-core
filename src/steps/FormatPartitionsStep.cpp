#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/FormatPartitionsStep.hpp>
#include <spacepi/liblinux/InstallationData.hpp>

using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void FormatPartitionsStep::run(InstallationData &data) {
    throw EXCEPTION(StateException("Not implemented"));
}
