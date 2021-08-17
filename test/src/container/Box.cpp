#include <gtest/gtest.h>
#include <spacepi/container/Box.hpp>
#include <spacepi/test/ElementType.hpp>
#include <spacepi/Parameter.hpp>

using namespace spacepi;
using namespace spacepi::container;
using namespace spacepi::test;

TEST(container_Box, ctors) {
    {
        Box<ElementType> b(NoConstruct);
        EXPECT_FALSE(b);
    }
    {
        Box<ElementType> b(42, ElementType::Tag);
        EXPECT_TRUE(b);
        EXPECT_EQ(b->getValue(), 42);
        EXPECT_EQ(b->getType(), ElementType::Tagged);

        Box<ElementType> copy(b);
        EXPECT_TRUE(copy);
        EXPECT_EQ(copy->getValue(), 42);
        EXPECT_EQ(copy->getType(), ElementType::Copy);

        Box<ElementType> move(Parameter::move<Box<ElementType>>(b));
        EXPECT_TRUE(move);
        EXPECT_EQ(move->getValue(), 42);
        EXPECT_EQ(move->getType(), ElementType::Move);
        EXPECT_EQ(b->getType(), ElementType::Moved);
    }
}

TEST(container_Box, ctorDtor) {
    Box<ElementType> b(NoConstruct);
    EXPECT_FALSE(b);

    b.construct(42);
    EXPECT_TRUE(b);
    EXPECT_EQ(b->getValue(), 42);
    EXPECT_EQ(b->getType(), ElementType::Init);

    b.destruct();
    EXPECT_FALSE(b);
}
