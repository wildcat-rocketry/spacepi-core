#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/PrepareNFSStep.hpp>
#include <spacepi/liblinux/InstallationData.hpp>

using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void PrepareNFSStep::run(InstallationData &data) {
    throw EXCEPTION(NotImplementedException("PrepareNFSStep not implemented"));
}
