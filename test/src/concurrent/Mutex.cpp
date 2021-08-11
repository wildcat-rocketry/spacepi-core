#include <gtest/gtest.h>
#include <spacepi/concurrent/Mutex.hpp>
#include <spacepi/concurrent/SleepMode.hpp>

using namespace spacepi::concurrent;

TEST(spacepi_Concurrent_Mutex, Fast) {
    Mutex<Fast> mtx;
    EXPECT_TRUE(mtx.tryLock());
    mtx.unlock();
    EXPECT_TRUE(mtx.tryLock());
    EXPECT_FALSE(mtx.tryLock());
    mtx.unlock();
    EXPECT_TRUE(mtx.tryLock());
}
