#include <spacepi/liblinux/steps/BuildSpacePiStep.hpp>
#include <spacepi/liblinux/steps/CleanupStep.hpp>
#include <spacepi/liblinux/steps/CloneGitRepoStep.hpp>
#include <spacepi/liblinux/steps/EnsureRootStep.hpp>
#include <spacepi/liblinux/steps/FormatPartitionsStep.hpp>
#include <spacepi/liblinux/steps/InitStateStep.hpp>
#include <spacepi/liblinux/steps/InitStep.hpp>
#include <spacepi/liblinux/steps/InstallBaseSystemStep.hpp>
#include <spacepi/liblinux/steps/InstallSpacePiStep.hpp>
#include <spacepi/liblinux/steps/InstallSystemFilesStep.hpp>
#include <spacepi/liblinux/steps/MountPartitionsStep.hpp>
#include <spacepi/liblinux/DefaultInstallationPlan.hpp>

using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

DefaultInstallationPlan::DefaultInstallationPlan() noexcept {
    appendStep<InitStateStep>();
    appendStep<EnsureRootStep>();
    appendStep<InitStep>();
    appendStep<FormatPartitionsStep>();
    appendStep<MountPartitionsStep>();
    appendStep<InstallBaseSystemStep>();
    appendStep<InstallSystemFilesStep>();
    appendStep<CloneGitRepoStep>();
    appendStep<BuildSpacePiStep>();
    appendStep<InstallSpacePiStep>();
    appendStep<CleanupStep>();
}
