#include <functional>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <boost/asio.hpp>
#include <boost/config.hpp>
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

io_context &NetworkThread::getContext() noexcept {
    return ctx;
}

void NetworkThread::start() {
    std::unique_lock<mutex> lck(mtx);
    switch (state) {
        case Init:
            state = FirstStart;
            break;
        case Waiting:
            state = MultipleStart;
            break;
        case FirstStart:
        case MultipleStart:
        case Running:
            return;
        case Stopping:
            log(LogLevel::Error) << "NetworkThread start requested after thread has been shut down.";
            return;
        default:
            log(LogLevel::Error) << "Invalid NetworkThread state while starting: " << state;
            return;
    }
    cond.notify_one();
}

void NetworkThread::join() {
    start();
    thread.join();
}

void NetworkThread::stop() {
    std::unique_lock<mutex> lck(mtx);
    enum State oldState = state;
    state = Stopping;
    switch (oldState) {
        case Init:
        case FirstStart:
        case Waiting:
        case MultipleStart:
            cond.notify_one();
            lck.unlock();
            break;
        case Running:
            lck.unlock();
            ctx.stop();
            break;
        case Stopping:
            return;
        default:
            log(LogLevel::Error) << "Invalid NetworkThread state while stopping: " << state;
            cond.notify_one();
            lck.unlock();
            ctx.stop();
            break;
    }
    if (thread.joinable()) {
        join();
    }
}

NetworkThread::NetworkThread() : state(Init), thread(bind(&NetworkThread::run, this)) {
}

NetworkThread::~NetworkThread() {
    stop();
}

void NetworkThread::run() {
    std::unique_lock<mutex> lck(mtx);
    while (state != Stopping) {
        switch (state) {
            case Init:
            case Waiting:
                cond.wait(lck);
                break;
            case FirstStart:
                log(LogLevel::Info) << "Network thread starting...";
                BOOST_FALLTHROUGH;
            case MultipleStart:
                state = Running;
                lck.unlock();
                try {
                    ctx.run();
                } catch (const std::exception &) {
                    log(LogLevel::Error) << Exception::getPointer();
                }
                lck.lock();
                if (state != Stopping) {
                    state = Waiting;
                }
                break;
            case Stopping:
                break;
            default:
                log(LogLevel::Error) << "Invalid NetworkThread state: " << state;
                break;
        }
    }
    log(LogLevel::Info) << "Network thread shutting down.";
}
