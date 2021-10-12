#include <chrono>
#include <thread>
#include <vector>
#include <gtest/gtest.h>
#include <spacepi/Platform.hpp>

using namespace std;
using namespace std::chrono;
using namespace spacepi;

static void threadFunc(int *lock, int *accum, int idx);

static constexpr int numThreads = 10;
static constexpr int numIters = 100;

TEST(spacepi_Platform, testAndSet) {
    vector<thread> threads;
    threads.reserve(numThreads);
    int lock = 0;
    int accum = 0;
    while (threads.size() < threads.capacity()) {
        threads.emplace_back(threadFunc, &lock, &accum, threads.size());
    }
    for (vector<thread>::iterator it = threads.begin(); it != threads.end(); ++it) {
        it->join();
    }
    EXPECT_EQ(accum, threads.size() * numIters);
}

static void threadFunc(int *lock, int *accum, int idx) {
    for (int i = 0; i < numIters; ++i) {
        while (Platform::testAndSet(*lock))
            ;
        int val = *accum + 1;
        this_thread::sleep_for(microseconds((idx ^ val) % 10));
        *accum = val;
        Platform::unset(*lock);
    }
}
