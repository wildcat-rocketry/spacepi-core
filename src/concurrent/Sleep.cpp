#include <boost/fiber/all.hpp>
#include <spacepi/concurrent/ConditionVariable.hpp>
#include <spacepi/concurrent/Sleep.hpp>

using namespace boost::fibers;
using namespace spacepi::concurrent;
using namespace spacepi::concurrent::detail;

SleepPredicate SleepPredicate::instance;
ConditionVariable Sleep::cond;

bool SleepPredicate::operator ()() const noexcept {
    return false;
}
