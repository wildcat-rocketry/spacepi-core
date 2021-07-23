#include <gtest/gtest.h>
#include <spacepi/container/Function.hpp>

using namespace spacepi::container;

class MultiplyFunctoid {
    public:
        constexpr MultiplyFunctoid(int factor) noexcept : factor(factor) {
        }

        constexpr int operator ()(int factor) noexcept {
            return factor * this->factor;
        }

        constexpr int operator ()(int factor) const noexcept {
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
    ASSERT_FALSE(Function<void()>());
    ASSERT_FALSE(Function<void()>(nullptr));
    ASSERT_FALSE(Function<int(int)>(doubleInt) = nullptr);
    ASSERT_ANY_THROW(Function<void()>()());
}

TEST(container_Function, function) {
    ASSERT_TRUE(Function<int(int)>() = doubleInt);
    ASSERT_TRUE(Function<int(int)>(doubleInt));
    ASSERT_TRUE(Function<int(int)>(doubleInt) = tripleInt);
    ASSERT_FALSE(Function<int(int)>(doubleInt) = nullptr);
    ASSERT_EQ(Function<int(int)>(doubleInt)(2), 4);
    ASSERT_EQ((Function<int(int)>(doubleInt) = tripleInt)(2), 6);
    ASSERT_EQ((Function<int(int)>(doubleInt) = Function<int(int)>(tripleInt))(2), 6);
    ASSERT_EQ((Function<int(int)>(nullptr) = doubleInt)(2), 4);
}

TEST(container_Function, functoid) {
    MultiplyFunctoid m4(4);
    MultiplyFunctoid m5(5);
    ASSERT_TRUE(Function<int(int)>() = m4);
    ASSERT_TRUE(Function<int(int)>(m4));
    ASSERT_TRUE(Function<int(int)>(m4) = m5);
    ASSERT_FALSE(Function<int(int)>(m4) = nullptr);
    ASSERT_EQ(Function<int(int)>(m4)(2), 8);
    ASSERT_EQ((Function<int(int)>(m4) = m5)(2), 10);
    ASSERT_EQ((Function<int(int)>(m4) = Function<int(int)>(m5))(2), 10);
    ASSERT_EQ((Function<int(int)>(nullptr) = m4)(2), 8);
}

TEST(container_Function, constFunctoid) {
    const MultiplyFunctoid m6(6);
    const MultiplyFunctoid m7(7);
    ASSERT_TRUE(Function<int(int)>() = m6);
    ASSERT_TRUE(Function<int(int)>(m6));
    ASSERT_TRUE(Function<int(int)>(m6) = m7);
    ASSERT_FALSE(Function<int(int)>(m6) = nullptr);
    ASSERT_EQ(Function<int(int)>(m6)(2), -12);
    ASSERT_EQ((Function<int(int)>(m6) = m7)(2), -14);
    ASSERT_EQ((Function<int(int)>(m6) = Function<int(int)>(m7))(2), -14);
    ASSERT_EQ((Function<int(int)>(nullptr) = m6)(2), -12);
}
