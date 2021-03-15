#include <spacepi/liblinux/steps/CleanupStep.hpp>
#include <spacepi/liblinux/steps/CloneImageStep.hpp>
#include <spacepi/liblinux/steps/ConnectStep.hpp>
#include <spacepi/liblinux/steps/EnsureRootStep.hpp>
#include <spacepi/liblinux/steps/InitStateStep.hpp>
#include <spacepi/liblinux/steps/MountPartitionsStep.hpp>
#include <spacepi/liblinux/steps/PrepareNFSStep.hpp>
#include <spacepi/liblinux/steps/SSHTunnelStep.hpp>
#include <spacepi/liblinux/DefaultConnectPlan.hpp>

using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

DefaultConnectPlan::DefaultConnectPlan() noexcept {
    appendStep<InitStateStep>();
    appendStep<SSHTunnelStep>();
    appendStep<EnsureRootStep>();
    appendStep<CloneImageStep>();
    appendStep<MountPartitionsStep>();
    appendStep<PrepareNFSStep>();
    appendStep<ConnectStep>();
    appendStep<CleanupStep>();
}
