#include <functional>
#include <memory>
#include <thread>
#include <vector>
#include <boost/fiber/all.hpp>
#include <spacepi/concurrent/ThreadPool.hpp>
#include <spacepi/messaging/network/NetworkThread.hpp>

using namespace std;
using namespace boost::fibers;
using namespace spacepi::concurrent;
using namespace spacepi::messaging::network;

ThreadPool::ThreadPool() {
}

ThreadPool::~ThreadPool() {
    thread th(bind(&ThreadPool::run, this));
    NetworkThread::instance.start();
    th.join();
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
    NetworkThread::instance.join();
}
