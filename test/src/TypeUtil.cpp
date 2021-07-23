#include <gtest/gtest.h>
#include <spacepi/TypeUtil.hpp>
#include <spacepi/test/TypeChecker.hpp>

using namespace spacepi;
using namespace spacepi::test;

TEST(TypeUtil, NoRef) {
    ASSERT_EQ(TypeChecker::IntCheck<TypeUtil<      int   >::NoRef>::Kind, TypeChecker::NonReference);
    ASSERT_EQ(TypeChecker::IntCheck<TypeUtil<      int & >::NoRef>::Kind, TypeChecker::NonReference);
    ASSERT_EQ(TypeChecker::IntCheck<TypeUtil<const int & >::NoRef>::Kind, TypeChecker::NonReference);
    ASSERT_EQ(TypeChecker::IntCheck<TypeUtil<      int &&>::NoRef>::Kind, TypeChecker::NonReference);
    ASSERT_EQ(TypeChecker::IntCheck<TypeUtil<const int &&>::NoRef>::Kind, TypeChecker::NonReference);
}
