#include <gtest/gtest.h>
#include <spacepi/concurrent/Lock.hpp>
#include <spacepi/concurrent/Mutex.hpp>
#include <spacepi/concurrent/SleepMode.hpp>
#include <spacepi/Parameter.hpp>

using namespace spacepi;
using namespace spacepi::concurrent;

TEST(spacepi_Concurrent_Lock, Fast) {
    Mutex<Fast> mtx;
    {
        Lock<Fast> lck(mtx);
        EXPECT_TRUE(lck);
        lck.lock();
        EXPECT_TRUE(lck);
        EXPECT_TRUE(lck.tryLock());
        EXPECT_FALSE(mtx.tryLock());
        lck.unlock();
        EXPECT_FALSE(lck);
        lck.adopt();
        EXPECT_TRUE(lck);
        lck.release();
        EXPECT_FALSE(lck);
        lck.lock();
        EXPECT_TRUE(lck);
    }
    EXPECT_TRUE(mtx.tryLock());
    {
        Lock<Fast> lck(mtx, TryLock);
        EXPECT_FALSE(lck);
    }
    EXPECT_FALSE(mtx.tryLock());
    mtx.unlock();
    {
        Lock<Fast> lck(mtx, TryLock);
        EXPECT_TRUE(lck);
    }
    EXPECT_TRUE(mtx.tryLock());
    {
        Lock<Fast> lck(mtx, DeferLock);
        EXPECT_FALSE(lck);
    }
    mtx.unlock();
    {
        Lock<Fast> lck(mtx, DeferLock);
        EXPECT_FALSE(lck);
        EXPECT_TRUE(lck.tryLock());
    }
    {
        Lock<Fast> lck(mtx);
        lck.release();
    }
    EXPECT_FALSE(mtx.tryLock());
    {
        Lock<Fast> lck(mtx, AdoptLock);
        EXPECT_TRUE(lck);
    }
    EXPECT_TRUE(mtx.tryLock());
    mtx.unlock();
    {
        Lock<Fast> lck2(mtx, DeferLock);
        {
            Lock<Fast> lck(mtx);
            EXPECT_TRUE(lck);
            lck2 = Parameter::move<Lock<Fast>>(lck);
            EXPECT_FALSE(lck);
        }
        EXPECT_FALSE(mtx.tryLock());
        EXPECT_TRUE(lck2);
    }
    EXPECT_TRUE(mtx.tryLock());
    mtx.unlock();
    {
        Lock<Fast> lck(mtx);
        EXPECT_TRUE(lck);
        Lock<Fast> lck2(Parameter::move<Lock<Fast>>(lck));
        EXPECT_FALSE(lck);
        EXPECT_TRUE(lck2);
    }
}
