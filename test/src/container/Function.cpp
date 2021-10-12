#include <gtest/gtest.h>
#include <spacepi/container/Function.hpp>

using namespace spacepi::container;

class MultiplyFunctoid {
    public:
        constexpr MultiplyFunctoid(int factor) noexcept
            : factor(factor) {
        }

        constexpr int operator()(int factor) noexcept {
            return factor * this->factor;
        }

        constexpr int operator()(int factor) const noexcept {
            return -factor * this->factor;
        }

    private:
        int factor;
};

static void doubleInt();
static void tripleInt();

static int doubleInt(int in) {
    return in * 2;
}

static int tripleInt(int in) {
    return in * 3;
}

TEST(container_Function, null) {
    EXPECT_FALSE(Function<void()>());
    EXPECT_FALSE(Function<void()>(nullptr));
    EXPECT_FALSE(Function<int(int)>(doubleInt) = nullptr);
    EXPECT_ANY_THROW(Function<void()>()());
}

TEST(container_Function, function) {
    EXPECT_TRUE(Function<int(int)>() = doubleInt);
    EXPECT_TRUE(Function<int(int)>(doubleInt));
    EXPECT_TRUE(Function<int(int)>(doubleInt) = tripleInt);
    EXPECT_FALSE(Function<int(int)>(doubleInt) = nullptr);
    EXPECT_EQ(Function<int(int)>(doubleInt)(2), 4);
    EXPECT_EQ((Function<int(int)>(doubleInt) = tripleInt)(2), 6);
    EXPECT_EQ((Function<int(int)>(doubleInt) = Function<int(int)>(tripleInt))(2), 6);
    EXPECT_EQ((Function<int(int)>(nullptr) = doubleInt)(2), 4);
}

TEST(container_Function, functoid) {
    MultiplyFunctoid m4(4);
    MultiplyFunctoid m5(5);
    EXPECT_TRUE(Function<int(int)>() = m4);
    EXPECT_TRUE(Function<int(int)>(m4));
    EXPECT_TRUE(Function<int(int)>(m4) = m5);
    EXPECT_FALSE(Function<int(int)>(m4) = nullptr);
    EXPECT_EQ(Function<int(int)>(m4)(2), 8);
    EXPECT_EQ((Function<int(int)>(m4) = m5)(2), 10);
    EXPECT_EQ((Function<int(int)>(m4) = Function<int(int)>(m5))(2), 10);
    EXPECT_EQ((Function<int(int)>(nullptr) = m4)(2), 8);
}

TEST(container_Function, constFunctoid) {
    const MultiplyFunctoid m6(6);
    const MultiplyFunctoid m7(7);
    EXPECT_TRUE(Function<int(int)>() = m6);
    EXPECT_TRUE(Function<int(int)>(m6));
    EXPECT_TRUE(Function<int(int)>(m6) = m7);
    EXPECT_FALSE(Function<int(int)>(m6) = nullptr);
    EXPECT_EQ(Function<int(int)>(m6)(2), -12);
    EXPECT_EQ((Function<int(int)>(m6) = m7)(2), -14);
    EXPECT_EQ((Function<int(int)>(m6) = Function<int(int)>(m7))(2), -14);
    EXPECT_EQ((Function<int(int)>(nullptr) = m6)(2), -12);
}

TEST(container_Function, lambda) {
    EXPECT_TRUE(Function<int(int)>() = [](int in) -> int { return in * 8; });
    EXPECT_TRUE(Function<int(int)>([](int in) -> int { return in * 8; }));
    EXPECT_TRUE(Function<int(int)>([](int in) -> int { return in * 8; }) = [](int in) -> int { return in * 9; });
    EXPECT_FALSE(Function<int(int)>([](int in) -> int { return in * 8; }) = nullptr);
    EXPECT_EQ(Function<int(int)>([](int in) -> int { return in * 8; })(2), 16);
    EXPECT_EQ((Function<int(int)>([](int in) -> int { return in * 8; }) = [](int in) -> int { return in * 9; })(2), 18);
    EXPECT_EQ((Function<int(int)>([](int in) -> int { return in * 8; }) = Function<int(int)>([](int in) -> int { return in * 9; }))(2), 18);
    EXPECT_EQ((Function<int(int)>(nullptr) = [](int in) -> int { return in * 8; })(2), 16);
}
