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
    vector<string> args = CommandConfigurable::parse(argc, argv);
    Connection conn(args);
    ThreadPool {
        bind(helloReceiver, conn),
        bind(goodbyeReceiver, conn)
    };
    return 0;
}

static void helloReceiver(Connection &conn) {
    Logger log("example:subscriber");
    Subscription<HelloMessage> sub(&conn);
    HelloMessage msg;
    while (true) {
        sub >> msg;
        log(LogLevel::Info) << "Got greeting '" << msg.greeting() << "' from " << msg.name();
    }
}

static void goodbyeReceiver(Connection &conn) {
    Logger log("example:subscriber");
    Subscription<GoodbyeMessage> sub(&conn);
    GoodbyeMessage msg;
    while (true) {
        sub >> msg;
        log(LogLevel::Info) << "Got goodbye '" << msg.message() << "'";
    }
}
