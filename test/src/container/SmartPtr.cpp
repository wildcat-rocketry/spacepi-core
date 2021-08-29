#include <gtest/gtest.h>
#include <spacepi/container/AllocationType.hpp>
#include <spacepi/container/SmartPtr.hpp>
#include <spacepi/test/ElementType.hpp>
#include <spacepi/Parameter.hpp>

using namespace spacepi;
using namespace spacepi::container;
using namespace spacepi::test;

TEST(container_SmartPtr, Static) {
    SmartPtr<ElementType, Static> ptr = nullptr;
    EXPECT_FALSE(ptr);

    ptr.reset(Construct, 42);
    EXPECT_TRUE(ptr);
    EXPECT_EQ(ptr->getValue(), 42);
    EXPECT_EQ(ptr->getType(), ElementType::Init);

    SmartPtr<ElementType, Static> copy(ptr);
    EXPECT_TRUE(copy);
    EXPECT_EQ(&*copy, (ElementType *) ptr);

    {
        SmartPtr<ElementType, Static> moved(Parameter::move<SmartPtr<ElementType, Static>>(ptr));
        EXPECT_TRUE(moved);
        EXPECT_TRUE(copy);
        EXPECT_FALSE(ptr);
        EXPECT_EQ(&*copy, &*moved);
    }

    SmartPtr<ElementType, Static> dummy(Construct);
    EXPECT_TRUE(dummy);
    EXPECT_EQ(dummy->getValue(), 0);
    EXPECT_NE(&*dummy, &*copy);
    EXPECT_TRUE(copy);
    EXPECT_EQ(copy->getValue(), 42);
    EXPECT_EQ(copy->getType(), ElementType::Move);
}
