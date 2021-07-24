#include <gtest/gtest.h>
#include <spacepi/container/AccessMode.hpp>
#include <spacepi/container/Property.hpp>
#include <spacepi/test/TypeChecker.hpp>

using namespace spacepi::container;
using namespace spacepi::test;

class TestData {
    public:
        int A;
        int B;
};

TEST(container_Property, read) {
    TestData data;
    data.A = 4;
    data.B = 2;
    Property<TestData, Read> mod([&data]() -> TestData & { return data; });
    Property<const TestData, Read> cst([&data]() -> const TestData & { return data; });
    Property<const TestData, Read> cpy([data]() -> const TestData & { return data; });
    data.A = 8;
    data.B = 6;
    EXPECT_EQ(((TestData) mod).A, 8);
    EXPECT_EQ(((TestData) mod).B, 6);
    EXPECT_EQ(mod->A, 8);
    EXPECT_EQ(mod->B, 6);
    EXPECT_EQ(((TestData) cst).A, 8);
    EXPECT_EQ(((TestData) cst).B, 6);
    EXPECT_EQ(cst->A, 8);
    EXPECT_EQ(cst->B, 6);
    EXPECT_EQ(((TestData) cpy).A, 4);
    EXPECT_EQ(((TestData) cpy).B, 2);
    EXPECT_EQ(cpy->A, 4);
    EXPECT_EQ(cpy->B, 2);
    int obj = 42;
    Property<int, Read> prop([&obj]() -> int { return obj; });
    EXPECT_EQ(TypeChecker::IntCheck<decltype(*prop.operator ->())>::Kind, TypeChecker::ModifiableLValue);
    EXPECT_EQ(TypeChecker::IntCheck<decltype(*((const Property<int, Read> &) prop).operator ->())>::Kind, TypeChecker::ConstLValue);
}

TEST(container_Property, write) {
    TestData data;
    data.A = 4;
    data.B = 2;
    Property<TestData, Write> prop([&data](const TestData &val) { data = val; });
    TestData val;
    val.A = 6;
    val.B = 4;
    prop = val;
    EXPECT_EQ(data.A, 6);
    EXPECT_EQ(data.B, 4);
}

TEST(container_Property, readWrite) {
    int data = 0;
    Property<int> prop([&data]() -> int { return data; }, [&data](int val) { data = val; });
    EXPECT_EQ(data, 0);
    EXPECT_EQ(prop = 42, 42);
    EXPECT_EQ(data, 42);
    Property<const int> cProp([&data]() -> const int { return data; }, [&data](int val) { data = val; });
    EXPECT_EQ(data, 42);
    EXPECT_EQ(cProp = 64, 64);
    EXPECT_EQ(data, 64);
}
