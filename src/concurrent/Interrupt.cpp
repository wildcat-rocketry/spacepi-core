#include <condition_variable>
#include <csignal>
#include <mutex>
#include <thread>
#include <boost/fiber/all.hpp>
#include <spacepi/concurrent/ConditionVariable.hpp>
#include <spacepi/concurrent/Interrupt.hpp>
#include <spacepi/messaging/network/NetworkThread.hpp>
#include <spacepi/util/Exception.hpp>

using namespace std;
using namespace boost;
using namespace spacepi::concurrent;
using namespace spacepi::concurrent::detail;
using namespace spacepi::messaging::network;
using namespace spacepi::util;


static void (*interruptSignalRegistration)(int) = signal(SIGINT, &Interrupt::interruptSignalHandler);
sig_atomic_t Interrupt::isCancelled = 0;
set<ThreadID> Interrupt::cancelDispatched;
mutex Interrupt::cancellationMutex;
condition_variable Interrupt::cancellationCond;
bool Interrupt::cancelling = false;
thread Interrupt::cancellationThread(&Interrupt::cancellationThreadFunc);

ThreadID::ThreadID() noexcept : tid(this_thread::get_id()), fid(this_fiber::get_id()) {
}

bool ThreadID::operator <(const ThreadID &other) const noexcept {
    if (tid < other.tid) {
        return true;
    } else if (tid > other.tid) {
        return false;
    } else {
        return fid < other.fid;
    }
}

void Interrupt::cancel() noexcept {
    if (isCancelled) {
        return;
    }
    isCancelled = 1;
    ConditionVariable::notify_global();
    NetworkThread::instance.stop();
}

void Interrupt::throwCancelled() {
    throw EXCEPTION(InterruptException("Program cancellation requested."));
}

void Interrupt::interruptSignalHandler(int sig) noexcept {
    unique_lock<mutex> lck(cancellationMutex);
    cancelling = true;
    cancellationCond.notify_all();
}

void Interrupt::cancellationThreadFunc() noexcept {
    unique_lock<mutex> lck(cancellationMutex);
    while (!cancelling) {
        cancellationCond.wait(lck);
    }
    lck.unlock();
    cancel();
}
