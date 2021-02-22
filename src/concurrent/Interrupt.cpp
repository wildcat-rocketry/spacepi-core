#include <condition_variable>
#include <csignal>
#include <mutex>
#include <thread>
#include <boost/fiber/all.hpp>
#include <spacepi/concurrent/AsyncInterrupt.hpp>
#include <spacepi/concurrent/Interrupt.hpp>
#include <spacepi/util/Exception.hpp>

using namespace std;
using namespace boost;
using namespace spacepi::concurrent;
using namespace spacepi::concurrent::detail;
using namespace spacepi::util;

const InterruptException Interrupt::cancelException("Program cancellation requested.");
Interrupt Interrupt::instance;

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
    if (instance.state == Cancelled) {
        return;
    }
    instance.state = Cancelled;
    AsyncInterrupt::fire();
}

void Interrupt::interruptSignalHandler(int sig) noexcept {
    unique_lock<mutex> lck(instance.mtx);
    instance.state = Cancelling;
    instance.cond.notify_all();
}

Interrupt::Interrupt() noexcept : state(Running), thread(bind(&Interrupt::cancellationThreadFunc, this)) {
    signal(SIGINT, &Interrupt::interruptSignalHandler);
}

Interrupt::~Interrupt() noexcept {
    unique_lock<mutex> lck(mtx);
    state = Stopping;
    cond.notify_all();
    lck.unlock();
    thread.join();
}

void Interrupt::throwCancelled() {
    ThreadID id;
    set<ThreadID>::const_iterator it = dispatched.find(id);
    if (it == dispatched.end()) {
        dispatched.emplace_hint(it, id);
        throw EXCEPTION(InterruptException(cancelException));
    }
}

void Interrupt::cancellationThreadFunc() noexcept {
    unique_lock<mutex> lck(mtx);
    while (state == Running) {
        cond.wait(lck);
    }
    lck.unlock();
    if (state == Cancelling) {
        cancel();
    }
}
