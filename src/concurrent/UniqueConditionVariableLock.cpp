#include <mutex>
#include <utility>
#include <spacepi/concurrent/ConditionVariable.hpp>
#include <spacepi/concurrent/UniqueConditionVariableLock.hpp>

using namespace std;
using namespace boost;
using namespace spacepi::concurrent;

UniqueConditionVariableLock::UniqueConditionVariableLock(ConditionVariable &mtx) {
    pair<std::mutex *, fibers::mutex *> impl = mtx.mutex();
    if (impl.first) {
        slck = unique_lock<std::mutex>(*impl.first);
    } else if (impl.second) {
        blck = unique_lock<fibers::mutex>(*impl.second);
    }
}

UniqueConditionVariableLock::UniqueConditionVariableLock(ConditionVariable &mtx, try_to_lock_t tag) {
    pair<std::mutex *, fibers::mutex *> impl = mtx.mutex();
    if (impl.first) {
        slck = unique_lock<std::mutex>(*impl.first, tag);
    } else if (impl.second) {
        blck = unique_lock<fibers::mutex>(*impl.second, tag);
    }
}

UniqueConditionVariableLock::UniqueConditionVariableLock(ConditionVariable &mtx, defer_lock_t tag) {
    pair<std::mutex *, fibers::mutex *> impl = mtx.mutex();
    if (impl.first) {
        slck = unique_lock<std::mutex>(*impl.first, tag);
    } else if (impl.second) {
        blck = unique_lock<fibers::mutex>(*impl.second, tag);
    }
}

UniqueConditionVariableLock::UniqueConditionVariableLock(ConditionVariable &mtx, adopt_lock_t tag) {
    pair<std::mutex *, fibers::mutex *> impl = mtx.mutex();
    if (impl.first) {
        slck = unique_lock<std::mutex>(*impl.first, tag);
    } else if (impl.second) {
        blck = unique_lock<fibers::mutex>(*impl.second, tag);
    }
}

UniqueConditionVariableLock::UniqueConditionVariableLock(UniqueConditionVariableLock &&move) {
    *this = std::move(move);
}

UniqueConditionVariableLock &UniqueConditionVariableLock::operator =(UniqueConditionVariableLock &&move) {
    slck = std::move(move.slck);
    blck = std::move(move.blck);
    return *this;
}

void UniqueConditionVariableLock::lock() {
    if (slck.mutex()) {
        slck.lock();
    } else if (blck.mutex()) {
        blck.lock();
    }
}

bool UniqueConditionVariableLock::try_lock() {
    if (slck.mutex()) {
        return slck.try_lock();
    } else if (blck.mutex()) {
        return blck.try_lock();
    } else {
        return false;
    }
}

void UniqueConditionVariableLock::unlock() {
    if (slck.mutex()) {
        slck.unlock();
    } else if (blck.mutex()) {
        blck.unlock();
    }
}

pair<std::mutex *, fibers::mutex *> UniqueConditionVariableLock::mutex() const noexcept {
    return make_pair(slck.mutex(), blck.mutex());
}

UniqueConditionVariableLock::operator unique_lock<std::mutex> &() noexcept {
    return slck;
}

UniqueConditionVariableLock::operator unique_lock<fibers::mutex> &() noexcept {
    return blck;
}

bool UniqueConditionVariableLock::owns_lock() const noexcept {
    return slck.owns_lock() || blck.owns_lock();
}

UniqueConditionVariableLock::operator bool() const noexcept {
    return slck || blck;
}

pair<std::mutex *, fibers::mutex *> UniqueConditionVariableLock::release() noexcept {
    return make_pair(slck.release(), blck.release());
}
