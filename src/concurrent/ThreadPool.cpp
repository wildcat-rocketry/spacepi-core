#include <functional>
#include <memory>
#include <thread>
#include <vector>
#include <boost/fiber/all.hpp>
#include <spacepi/concurrent/ThreadPool.hpp>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

using namespace std;
using namespace boost::fibers;
using namespace spacepi::concurrent;
using namespace spacepi::concurrent::detail;
using namespace spacepi::util;

ThreadPool::ThreadPool(Command &cmd) : CommandConfigurable("", cmd) {
}

ThreadPool::~ThreadPool() {
    if (thread.joinable()) {
        thread.join();
    }
}

void ThreadPool::runCommand() {
    thread = std::thread(bind(&ThreadPool::run, this));
}

void ThreadPool::run() {
    vector<fiber> fibers;
    fibers.reserve(functions.size());
    for (vector<unique_ptr<GenericFiber>>::const_iterator it = functions.begin(); it != functions.end(); ++it) {
        (*it)->start(fibers);
    }
    for (vector<fiber>::iterator it = fibers.begin(); it != fibers.end(); ++it) {
        it->join();
    }
}
