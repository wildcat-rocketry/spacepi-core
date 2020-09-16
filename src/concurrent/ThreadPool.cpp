#include <vector>
#include <boost/fiber/all.hpp>
#include <spacepi/concurrent/ThreadPool.hpp>

using namespace std;
using namespace boost::fibers;
using namespace spacepi::concurrent;

ThreadPool::ThreadPool() {
}

ThreadPool::~ThreadPool() {
    for (vector<fiber>::iterator it = functions.begin(); it != functions.end(); ++it) {
        it->join();
    }
}
