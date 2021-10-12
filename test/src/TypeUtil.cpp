#include <gtest/gtest.h>
#include <spacepi/Ints.hpp>
#include <spacepi/test/TypeChecker.hpp>
#include <spacepi/TypeUtil.hpp>

using namespace spacepi;
using namespace spacepi::test;

TEST(TypeUtil, NoRef) {
    EXPECT_EQ(TypeChecker::IntCheck<TypeUtil<int>::NoRef>::Kind, TypeChecker::NonReference);
    EXPECT_EQ(TypeChecker::IntCheck<TypeUtil<int &>::NoRef>::Kind, TypeChecker::NonReference);
    EXPECT_EQ(TypeChecker::IntCheck<TypeUtil<const int &>::NoRef>::Kind, TypeChecker::NonReference);
    EXPECT_EQ(TypeChecker::IntCheck<TypeUtil<int &&>::NoRef>::Kind, TypeChecker::NonReference);
    EXPECT_EQ(TypeChecker::IntCheck<TypeUtil<const int &&>::NoRef>::Kind, TypeChecker::NonReference);
}

TEST(TypeUtil, isPrimitive) {
    EXPECT_TRUE(TypeUtil<char>::isPrimitive());
    EXPECT_TRUE(TypeUtil<int>::isPrimitive());
    EXPECT_TRUE(TypeUtil<long double>::isPrimitive());
    EXPECT_TRUE(TypeUtil<int &>::isPrimitive());
    EXPECT_FALSE(TypeUtil<TypeChecker::CompositeType>::isPrimitive());
    EXPECT_FALSE(TypeUtil<TypeChecker::CompositeType &>::isPrimitive());
    EXPECT_TRUE(TypeUtil<TypeChecker::CompositeType *>::isPrimitive());
}

TEST(TypeUtil, PrimitiveOrReference) {
    EXPECT_EQ(TypeChecker::IntCheck<TypeUtil<int>::PrimitiveOrReference>::Kind, TypeChecker::NonReference);
    EXPECT_EQ(TypeChecker::IntCheck<TypeUtil<int &>::PrimitiveOrReference>::Kind, TypeChecker::NonReference);
    EXPECT_EQ(TypeChecker::CompositeCheck<TypeUtil<TypeChecker::CompositeType>::PrimitiveOrReference>::Kind, TypeChecker::ModifiableLValue);
    EXPECT_EQ(TypeChecker::CompositeCheck<TypeUtil<TypeChecker::CompositeType &&>::PrimitiveOrReference>::Kind, TypeChecker::ModifiableLValue);
}
