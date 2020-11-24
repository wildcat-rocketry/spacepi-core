#include <cstdlib>
#include <spacepi/messaging/network/NetworkThread.hpp>
#include <spacepi/router/Router.hpp>
#include <spacepi/util/Command.hpp>

using namespace spacepi::messaging::network;
using namespace spacepi::router;
using namespace spacepi::util;

int main(int argc, const char **argv) {
    Command cmd(argc, argv);
    Router router(cmd);
    if (cmd.run()) {
        NetworkThread::instance.join();
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}
