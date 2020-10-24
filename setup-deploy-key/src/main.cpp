#include <spacepi/messaging/network/NetworkThread.hpp>
#include <spacepi/setup-deploy-key/Server.hpp>

using namespace spacepi::messaging::network;
using namespace spacepi::target::deployKey;

int main(int argc, const char **argv) {
    Server server(1234);
    NetworkThread::instance.join();

    return 0;
}

