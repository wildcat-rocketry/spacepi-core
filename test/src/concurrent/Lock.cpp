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
        ASSERT_TRUE(lck);
        lck.lock();
        ASSERT_TRUE(lck);
        ASSERT_TRUE(lck.tryLock());
        ASSERT_FALSE(mtx.tryLock());
        lck.unlock();
        ASSERT_FALSE(lck);
        lck.adopt();
        ASSERT_TRUE(lck);
        lck.release();
        ASSERT_FALSE(lck);
        lck.lock();
        ASSERT_TRUE(lck);
    }
    ASSERT_TRUE(mtx.tryLock());
    {
        Lock<Fast> lck(mtx, TryLock);
        ASSERT_FALSE(lck);
    }
    ASSERT_FALSE(mtx.tryLock());
    mtx.unlock();
    {
        Lock<Fast> lck(mtx, TryLock);
        ASSERT_TRUE(lck);
    }
    ASSERT_TRUE(mtx.tryLock());
    {
        Lock<Fast> lck(mtx, DeferLock);
        ASSERT_FALSE(lck);
    }
    mtx.unlock();
    {
        Lock<Fast> lck(mtx, DeferLock);
        ASSERT_FALSE(lck);
        ASSERT_TRUE(lck.tryLock());
    }
    {
        Lock<Fast> lck(mtx);
        lck.release();
    }
    ASSERT_FALSE(mtx.tryLock());
    {
        Lock<Fast> lck(mtx, AdoptLock);
        ASSERT_TRUE(lck);
    }
    ASSERT_TRUE(mtx.tryLock());
    mtx.unlock();
    {
        Lock<Fast> lck2(mtx, DeferLock);
        {
            Lock<Fast> lck(mtx);
            ASSERT_TRUE(lck);
            lck2 = Parameter::move<Lock<Fast>>(lck);
            ASSERT_FALSE(lck);
        }
        ASSERT_FALSE(mtx.tryLock());
        ASSERT_TRUE(lck2);
    }
    ASSERT_TRUE(mtx.tryLock());
    mtx.unlock();
    {
        Lock<Fast> lck(mtx);
        ASSERT_TRUE(lck);
        Lock<Fast> lck2(Parameter::move<Lock<Fast>>(lck));
        ASSERT_FALSE(lck);
        ASSERT_TRUE(lck2);
    }
}
