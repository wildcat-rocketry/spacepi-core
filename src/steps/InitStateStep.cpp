#include <string>
#include <spacepi/liblinux/steps/InitStateStep.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationOptions.hpp>
#include <spacepi/liblinux/State.hpp>

using namespace std;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void InitStateStep::run(InstallationData &data) {
    const string &serState = data.getData<InstallationOptions>().getState();
    if (serState.empty()) {
        data.initData<State>();
    } else {
        data.initData<State>(serState);
    }
}
