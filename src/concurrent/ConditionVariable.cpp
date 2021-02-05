#include <condition_variable>
#include <mutex>
#include <utility>
#include <boost/fiber/all.hpp>
#include <spacepi/concurrent/ConditionVariable.hpp>
#include <spacepi/concurrent/Interrupt.hpp>
#include <spacepi/concurrent/UniqueConditionVariableLock.hpp>

#include <spacepi/Log.hpp>

using namespace std;
using namespace boost;
using namespace boost::fibers;
using namespace spacepi::concurrent;

std::mutex ConditionVariable::gmtx;
ConditionVariable *ConditionVariable::head = nullptr;

ConditionVariable::ConditionVariable() noexcept : prev(nullptr), bwait(0) {
    unique_lock<std::mutex> lck(gmtx);
    next = head;
    head = this;
}

ConditionVariable::~ConditionVariable() noexcept {
    unique_lock<std::mutex> lck(gmtx);
    if (next) {
        next->prev = prev;
    }
    if (prev) {
        prev->next = next;
    } else {
        head = next;
    }
}

void ConditionVariable::notify_all() noexcept {
    scond.notify_all();
    bcond.notify_all();
}

void ConditionVariable::notify_one() noexcept {
    if (bwait) {
        bcond.notify_one();
    } else {
        scond.notify_one();
    }
}

void ConditionVariable::wait(UniqueConditionVariableLock &lck) {
    Interrupt::cancellationPoint();
    pair<std::mutex *, fibers::mutex *> mtx = lck.mutex();
    if (mtx.first) {
        scond.wait(lck);
    } else if (mtx.second) {
        ++bwait;
        try {
            bcond.wait(lck);
            --bwait;
        } catch (...) {
            --bwait;
            throw;
        }
    }
}

void ConditionVariable::lock() {
    unique_lock<std::mutex> lck(smtx);
    if (this_fiber::get_id() == fiber::id()) {
        lck.release();
    } else {
        bmtx.lock();
    }
}

bool ConditionVariable::try_lock() {
    unique_lock<std::mutex> lck(smtx, try_to_lock);
    if (lck) {
        if (this_fiber::get_id() == fiber::id()) {
            lck.release();
            return true;
        } else {
            return bmtx.try_lock();
        }
    } else {
        return false;
    }
}

void ConditionVariable::unlock() {
    if (this_fiber::get_id() == fiber::id()) {
        smtx.unlock();
    } else {
        bmtx.unlock();
    }
}

pair<std::mutex *, fibers::mutex *> ConditionVariable::mutex() noexcept {
    if (this_fiber::get_id() == fiber::id()) {
        return make_pair(&smtx, nullptr);
    } else {
        return make_pair(nullptr, &bmtx);
    }
}

void ConditionVariable::notify_global() noexcept {
    unique_lock<std::mutex> lck(gmtx);
    for (ConditionVariable *it = head; it; it = it->next) {
        UniqueConditionVariableLock clck(*it);
        it->notify_all();
    }
}
