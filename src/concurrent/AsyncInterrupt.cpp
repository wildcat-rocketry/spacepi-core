#include <mutex>
#include <spacepi/concurrent/AsyncInterrupt.hpp>

using namespace std;
using namespace spacepi::concurrent;

mutex AsyncInterrupt::mtx;
AsyncInterrupt *AsyncInterrupt::head = nullptr;

AsyncInterrupt::AsyncInterrupt() noexcept : prev(nullptr) {
    unique_lock<mutex> lck(mtx);
    next = head;
    head = this;
}

AsyncInterrupt::~AsyncInterrupt() {
    unique_lock<mutex> lck(mtx);
    if (next) {
        next->prev = prev;
    }
    if (prev) {
        prev->next = next;
    } else {
        head = next;
    }
}

AsyncInterrupt::AsyncInterrupt(const AsyncInterrupt &copy) noexcept : AsyncInterrupt() {
}

AsyncInterrupt &AsyncInterrupt::operator =(const AsyncInterrupt &copy) noexcept {
    return *this;
}

AsyncInterrupt::AsyncInterrupt(const AsyncInterrupt &&move) noexcept : AsyncInterrupt() {
}

AsyncInterrupt &AsyncInterrupt::operator =(const AsyncInterrupt &&move) noexcept {
    return *this;
}

void AsyncInterrupt::fire() noexcept {
    unique_lock<mutex> lck(mtx);
    for (AsyncInterrupt *it = head; it; it = it->next) {
        it->onCancel();
    }
}
