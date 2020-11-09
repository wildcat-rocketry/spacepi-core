#include <spacepi/messaging/network/NetworkThread.hpp>
#include <spacepi/setup-deploy-key/Server.hpp>
#include <spacepi/setup-deploy-key/ServerConn.hpp>
#include <spacepi/log/LogLevel.hpp>

using namespace spacepi::messaging::network;
using namespace spacepi::target::deployKey;
using namespace spacepi::log;

int main(int argc, const char **argv) {
    Server server(8000);
    NetworkThread::instance.start();

    server.waitForCallbackCode();
    std::string callbackcode = server.getCallbackCode();
    Logger log("verification");
    log(LogLevel::Info) << "Callback sent to main";
    log(LogLevel::Info) << callbackcode;

    NetworkThread::instance.join();

    return 0;
}

