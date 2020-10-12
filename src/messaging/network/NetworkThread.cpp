#include <functional>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <boost/asio.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/messaging/network/NetworkThread.hpp>
#include <spacepi/util/Exception.hpp>

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace spacepi::log;
using namespace spacepi::messaging::network;
using namespace spacepi::util;

NetworkThread NetworkThread::instance;

io_context &NetworkThread::getContext() {
    return ctx;
}

void NetworkThread::start() {
    std::unique_lock<mutex> lck(mtx);
    if (!hasStarted) {
        hasStarted = true;
        cond.notify_one();
    }
}

void NetworkThread::join() {
    start();
    thread.join();
}

void NetworkThread::stop() {
    ctx.stop();
    join();
}

NetworkThread::NetworkThread() : hasStarted(false), thread(bind(&NetworkThread::run, this)) {
}

NetworkThread::~NetworkThread() {
    if (thread.joinable()) {
        stop();
    }
}

void NetworkThread::run() {
    {
        std::unique_lock<mutex> lck(mtx);
        while (!hasStarted) {
            cond.wait(lck);
        }
    }
    log(LogLevel::Info) << "Network thread starting...";
    try {
        ctx.run();
    } catch (const std::exception &) {
        log(LogLevel::Error) << "" << Exception::createPointer(EXCEPTION(MessagingException("Error on networking thread")) << errinfo_nested_exception(Exception::getPointer()));
    }
    log(LogLevel::Info) << "Network thread shutting down.";
}
