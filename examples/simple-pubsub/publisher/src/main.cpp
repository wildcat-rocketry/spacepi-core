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

static void publishThread(Connection &conn);
static void helloReceiver(Connection &conn);
static void goodbyeReceiver(Connection &conn);
static void hiddenGoodbyeReceiver(Connection &conn);

int main(int argc, const char **argv) {
    vector<string> args = CommandConfigurable::parse(argc, argv);
    Connection conn(args);
    ThreadPool {
        bind(publishThread, conn),
        bind(helloReceiver, conn),
        bind(goodbyeReceiver, conn),
        bind(hiddenGoodbyeReceiver, conn)
    };
    return 0;
}

static void publishThread(Connection &conn) {
    Logger log("example:publisher");
    HelloMessage hello;
    GoodbyeMessage goodbye;

    log(LogLevel::Info) << "Sending hello from Zach";
    hello.set_greeting("Hello");
    hello.set_name("Zach");
    conn(0) << hello;

    log(LogLevel::Info) << "Sending hi from Miranda";
    hello.set_greeting("Hi");
    hello.set_name("Miranda");
    conn(0) << hello;

    log(LogLevel::Info) << "Sending message 'Goodbye world!'";
    goodbye.set_message("Goodbye world!");
    conn(0) << goodbye;

    log(LogLevel::Info) << "Sending message 'Goodbye hidden world!'";
    goodbye.set_message("Goodbye hidden world!");
    conn(1) << goodbye;

    log(LogLevel::Info) << "Sending message 'Goodbye very hidden world!'";
    goodbye.set_message("Goodbye very hidden world!");
    conn(2) << goodbye;
}

static void helloReceiver(Connection &conn) {
    Logger log("example:publisher");
    Subscription<HelloMessage> sub(&conn, 0);
    HelloMessage msg;
    while (true) {
        sub >> msg;
        log(LogLevel::Info) << "Got greeting '" << msg.greeting() << "' from " << msg.name();
    }
}

static void goodbyeReceiver(Connection &conn) {
    Logger log("example:publisher");
    Subscription<GoodbyeMessage> sub(&conn, 0);
    GoodbyeMessage msg;
    while (true) {
        sub >> msg;
        log(LogLevel::Info) << "Got goodbye '" << msg.message() << "'";
    }
}

static void hiddenGoodbyeReceiver(Connection &conn) {
    Logger log("example:publisher");
    Subscription<GoodbyeMessage> sub(&conn, 1);
    GoodbyeMessage msg;
    while (true) {
        sub >> msg;
        log(LogLevel::Info) << "Got hidden goodbye '" << msg.message() << "'";
    }
}
