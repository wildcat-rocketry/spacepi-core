#include <spacepi/Ints.hpp>
#include <gtest/gtest.h>

using namespace spacepi;

TEST(spacepi_ints, sizeof) {
    ASSERT_EQ(sizeof(u8 ), 1);
    ASSERT_EQ(sizeof(u16), 2);
    ASSERT_EQ(sizeof(u32), 4);
    ASSERT_EQ(sizeof(u64), 8);
    ASSERT_EQ(sizeof(i8 ), 1);
    ASSERT_EQ(sizeof(i16), 2);
    ASSERT_EQ(sizeof(i32), 4);
    ASSERT_EQ(sizeof(i64), 8);
}

TEST(spacepi_ints, sign) {
    ASSERT_GT((u8 ) -1, (u8 ) 0);
    ASSERT_GT((u16) -1, (u16) 0);
    ASSERT_GT((u32) -1, (u32) 0);
    ASSERT_GT((u64) -1, (u64) 0);
    ASSERT_LT((i8 ) -1, (i8 ) 0);
    ASSERT_LT((i16) -1, (i16) 0);
    ASSERT_LT((i32) -1, (i32) 0);
    ASSERT_LT((i64) -1, (i64) 0);
}
