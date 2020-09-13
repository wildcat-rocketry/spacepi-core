#include <initializer_list>
#include <boost/fiber/all.hpp>
#include <spacepi/concurrent/ThreadPool.hpp>
#include <spacepi/util/Exception.hpp>

using namespace std;
using namespace boost::fibers;
using namespace spacepi::concurrent;
using namespace spacepi::util;

GenericThreadPool::GenericThreadPool() : state(INIT) {
}

GenericThreadPool::~GenericThreadPool() {
    if (state == INIT) {
        run();
    }
}

void GenericThreadPool::run() {
    if (state != INIT) {
        throw EXCEPTION(StateException("Thread pool can only be run once."));
    }
    while (hasNext()) {
        // TODO
        moveNext();
    }
}

void GenericThreadPool::disable() {
    state = SPAWNED;
}
