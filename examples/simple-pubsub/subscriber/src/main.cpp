#include <cstdlib>
#include <functional>
#include <string>
#include <vector>
#include <spacepi/concurrent/ThreadPool.hpp>
#include <spacepi/example/GoodbyeMessage.pb.h>
#include <spacepi/example/HelloMessage.pb.h>
#include <spacepi/log/Logger.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/messaging/Connection.hpp>
#include <spacepi/messaging/Subscription.hpp>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

using namespace std;
using namespace spacepi::concurrent;
using namespace spacepi::example;
using namespace spacepi::log;
using namespace spacepi::messaging;
using namespace spacepi::util;

static void helloReceiver(Connection &conn);
static void goodbyeReceiver(Connection &conn);

int main(int argc, const char **argv) {
    Command cmd(argc, argv);
    Connection conn(cmd);
    ThreadPool pool(cmd, {
        bind(helloReceiver, conn),
        bind(goodbyeReceiver, conn)
    });
    if (cmd.run()) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}

static void helloReceiver(Connection &conn) {
    Logger log("example:subscriber");
    Subscription<HelloMessage> sub(conn, 0);
    HelloMessage msg;
    while (true) {
        sub >> msg;
        log(LogLevel::Info) << "Got greeting '" << msg.greeting() << "' from " << msg.name();
    }
}

static void goodbyeReceiver(Connection &conn) {
    Logger log("example:subscriber");
    Subscription<GoodbyeMessage> sub(conn, 0);
    GoodbyeMessage msg;
    while (true) {
        sub >> msg;
        log(LogLevel::Info) << "Got goodbye '" << msg.message() << "'";
    }
}
