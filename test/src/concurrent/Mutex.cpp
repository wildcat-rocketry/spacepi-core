#include <gtest/gtest.h>
#include <spacepi/concurrent/Mutex.hpp>
#include <spacepi/concurrent/SleepMode.hpp>

using namespace spacepi::concurrent;

TEST(spacepi_Concurrent_Mutex, Fast) {
    Mutex<Fast> mtx;
    ASSERT_TRUE(mtx.tryLock());
    mtx.unlock();
    ASSERT_TRUE(mtx.tryLock());
    ASSERT_FALSE(mtx.tryLock());
    mtx.unlock();
    ASSERT_TRUE(mtx.tryLock());
}
