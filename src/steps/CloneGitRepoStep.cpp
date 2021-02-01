#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/CloneGitRepoStep.hpp>
#include <spacepi/liblinux/InstallationData.hpp>

using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void CloneGitRepoStep::run(InstallationData &data) {
    throw EXCEPTION(StateException("Not implemented"));
}
