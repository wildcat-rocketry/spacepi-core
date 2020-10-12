#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>
#include <spacepi/concurrent/RWMutex.hpp>
#include <spacepi/log/ConsoleTarget.hpp>
#include <spacepi/log/Entry.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/log/LogManager.hpp>

using namespace std;
using namespace spacepi::concurrent;
using namespace spacepi::log;

LogManager LogManager::instance;

LogManager::LogManager() : running(true), thread(bind(&LogManager::run, this)) {
    *this += shared_ptr<LogTarget>(new ConsoleTarget(cout));
}

LogManager::~LogManager() {
    running = false;
    thread.join();
}

void LogManager::operator <<(const Entry &entry) {
    unique_lock<mutex> lck(entryMutex);
    entries.push(entry);
    cond.notify_one();
}

LogManager &LogManager::operator +=(shared_ptr<LogTarget> target) {
    unique_lock<RWMutex<mutex, unique_lock<mutex>, condition_variable>::WriteSide> lck(targetLock.write());
    targets.push_back(target);
    return *this;
}

void LogManager::run() {
    unique_lock<mutex> elck(entryMutex, defer_lock);
    unique_lock<RWMutex<mutex, unique_lock<mutex>, condition_variable>::ReadSide> tlck(targetLock.read(), defer_lock);
    while (true) {
        elck.lock();
        while (entries.empty() && running) {
            cond.wait(elck);
        }
        if (!running) {
            return;
        }
        Entry ent = entries.front();
        entries.pop();
        elck.unlock();
        tlck.lock();
        for (vector<shared_ptr<LogTarget>>::const_iterator it = targets.begin(); it != targets.end(); ++it) {
            (**it) << ent;
        }
        tlck.unlock();
    }
}
