#include <cstdlib>
#include <functional>
#include <spacepi/setup-deploy-key/DeployKeySetup.hpp>
#include <SpacePi.hpp>

using namespace std;
using namespace spacepi::concurrent;
using namespace spacepi::target::deployKey;
using namespace spacepi::util;
using namespace spacepi::messaging::network;

int main(int argc, const char **argv) {
    Command cmd(argc, argv);
    DeployKeySetup deploykeysetup(cmd);

    if (cmd.run()) {
        NetworkThread::instance.join();
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}
