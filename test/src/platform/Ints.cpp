#include <gtest/gtest.h>
#include <spacepi/Ints.hpp>
#include <spacepi/TypeUtil.hpp>

using namespace spacepi;

TEST(Platform_Ints, sizeof) {
    EXPECT_EQ(sizeof(u8 ), 1);
    EXPECT_EQ(sizeof(u16), 2);
    EXPECT_EQ(sizeof(u32), 4);
    EXPECT_EQ(sizeof(u64), 8);
    EXPECT_EQ(sizeof(i8 ), 1);
    EXPECT_EQ(sizeof(i16), 2);
    EXPECT_EQ(sizeof(i32), 4);
    EXPECT_EQ(sizeof(i64), 8);
}

TEST(Platform_Ints, sign) {
    EXPECT_GT((u8 ) -1, (u8 ) 0);
    EXPECT_GT((u16) -1, (u16) 0);
    EXPECT_GT((u32) -1, (u32) 0);
    EXPECT_GT((u64) -1, (u64) 0);
    EXPECT_LT((i8 ) -1, (i8 ) 0);
    EXPECT_LT((i16) -1, (i16) 0);
    EXPECT_LT((i32) -1, (i32) 0);
    EXPECT_LT((i64) -1, (i64) 0);
}

TEST(Platform_Ints, isPrimitive) {
    EXPECT_TRUE(TypeUtil<u8 >::isPrimitive);
    EXPECT_TRUE(TypeUtil<u16>::isPrimitive);
    EXPECT_TRUE(TypeUtil<u32>::isPrimitive);
    EXPECT_TRUE(TypeUtil<u64>::isPrimitive);
    EXPECT_TRUE(TypeUtil<i8 >::isPrimitive);
    EXPECT_TRUE(TypeUtil<i16>::isPrimitive);
    EXPECT_TRUE(TypeUtil<i32>::isPrimitive);
    EXPECT_TRUE(TypeUtil<i64>::isPrimitive);
}
