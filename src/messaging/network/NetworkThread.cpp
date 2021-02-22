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
    return *ctx;
}

void NetworkThread::start() noexcept {
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
        case Stopped:
            log(LogLevel::Error) << "NetworkThread start requested after thread has been shut down.";
            return;
        default:
            log(LogLevel::Error) << "Invalid NetworkThread state while starting: " << state;
            return;
    }
    cond.notify_all();
}

void NetworkThread::join() noexcept {
    std::unique_lock<mutex> lck(mtx);
    while (true) {
        switch (state) {
            case Init:
            case Waiting:
                state = Stopping;
                cond.notify_all();
                break;
            case FirstStart:
            case MultipleStart:
            case Running:
            case Stopping:
                cond.wait(lck);
                break;
            case Stopped:
                return;
            default:
                log(LogLevel::Error) << "Invalid NetworkThread state while joining: " << state;
                return;
        }
    }
}

void NetworkThread::stop() noexcept {
    std::unique_lock<mutex> lck(mtx);
    enum State oldState = state;
    state = Stopping;
    cond.notify_all();
    lck.unlock();
    switch (oldState) {
        case Init:
        case FirstStart:
        case Waiting:
        case MultipleStart:
        case Stopping:
            break;
        case Running:
            ctx->stop();
            break;
        case Stopped:
            return;
        default:
            log(LogLevel::Error) << "Invalid NetworkThread state while stopping: " << state;
            ctx->stop();
            break;
    }
    join();
}

NetworkThread::NetworkThread() : ctx(new io_context()), state(Init), thread(bind(&NetworkThread::run, this)) {
}

NetworkThread::~NetworkThread() {
    stop();
    if (thread.joinable()) {
        thread.join();
    }
}

void NetworkThread::onCancel() noexcept {
    stop();
}

void NetworkThread::run() noexcept {
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
                log(LogLevel::Debug) << "Network thread waking...";
                lck.unlock();
                try {
                    ctx->run();
                } catch (const std::exception &e) {
                    log(LogLevel::Error) << e.what() << ": " << Exception::getPointer();
                }
                ctx->restart();
                lck.lock();
                if (state != Stopping) {
                    state = Waiting;
                    cond.notify_all();
                }
                log(LogLevel::Debug) << "Network thread sleeping...";
                break;
            case Stopping:
                break;
            default:
                log(LogLevel::Error) << "Invalid NetworkThread state: " << state;
                break;
        }
    }
    log(LogLevel::Info) << "Network thread shutting down.";
    ctx.reset();
    state = Stopped;
    cond.notify_all();
}
