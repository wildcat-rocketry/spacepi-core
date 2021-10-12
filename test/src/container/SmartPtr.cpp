#include <gtest/gtest.h>
#include <spacepi/container/AllocationType.hpp>
#include <spacepi/container/SmartPtr.hpp>
#include <spacepi/Parameter.hpp>
#include <spacepi/test/ElementType.hpp>

using namespace spacepi;
using namespace spacepi::container;
using namespace spacepi::test;

TEST(container_SmartPtr, Static) {
    SmartPtr<ElementType, Static> ptr = nullptr;
    EXPECT_FALSE(ptr);
    EXPECT_TRUE(ptr.unique);

    ptr.reset(Construct, 42);
    EXPECT_TRUE(ptr);
    EXPECT_TRUE(ptr.unique);
    EXPECT_EQ(ptr->getValue(), 42);
    EXPECT_EQ(ptr->getType(), ElementType::Init);

    SmartPtr<ElementType, Static> copy(ptr);
    EXPECT_TRUE(copy);
    EXPECT_FALSE(copy.unique);
    EXPECT_FALSE(ptr.unique);
    EXPECT_EQ(&*copy, (ElementType *) ptr);

    {
        SmartPtr<ElementType, Static> moved(Parameter::move<SmartPtr<ElementType, Static>>(ptr));
        EXPECT_TRUE(moved);
        EXPECT_TRUE(copy);
        EXPECT_FALSE(ptr);
        EXPECT_FALSE(moved.unique);
        EXPECT_FALSE(copy.unique);
        EXPECT_TRUE(ptr.unique);
        EXPECT_EQ(&*copy, &*moved);
    }

    SmartPtr<ElementType, Static> dummy(Construct);
    EXPECT_TRUE(dummy);
    EXPECT_FALSE(dummy.unique);
    EXPECT_EQ(dummy->getValue(), 0);
    EXPECT_NE(&*dummy, &*copy);
    EXPECT_TRUE(copy);
    EXPECT_FALSE(copy.unique);
    EXPECT_EQ(copy->getValue(), 42);
    EXPECT_EQ(copy->getType(), ElementType::Move);
}

TEST(container_SmartPtr, Dynamic) {
    SmartPtr<ElementType, Dynamic> ptr = nullptr;
    EXPECT_FALSE(ptr);
    EXPECT_TRUE(ptr.unique);

    ptr.reset(Construct, 42);
    EXPECT_TRUE(ptr);
    EXPECT_TRUE(ptr.unique);
    EXPECT_EQ(ptr->getValue(), 42);
    EXPECT_EQ(ptr->getType(), ElementType::Init);

    SmartPtr<ElementType, Dynamic> copy(ptr);
    EXPECT_TRUE(copy);
    EXPECT_FALSE(copy.unique);
    EXPECT_FALSE(ptr.unique);
    EXPECT_EQ(&*copy, (ElementType *) ptr);

    {
        SmartPtr<ElementType, Dynamic> moved(Parameter::move<SmartPtr<ElementType, Dynamic>>(ptr));
        EXPECT_TRUE(moved);
        EXPECT_TRUE(copy);
        EXPECT_FALSE(ptr);
        EXPECT_FALSE(moved.unique);
        EXPECT_FALSE(copy.unique);
        EXPECT_TRUE(ptr.unique);
        EXPECT_EQ(&*copy, &*moved);
    }

    SmartPtr<ElementType, Dynamic> dummy(Construct);
    EXPECT_TRUE(dummy);
    EXPECT_FALSE(dummy.unique);
    EXPECT_EQ(dummy->getValue(), 0);
    EXPECT_NE(&*dummy, &*copy);
    EXPECT_TRUE(copy);
    EXPECT_FALSE(copy.unique);
    EXPECT_EQ(copy->getValue(), 42);
    EXPECT_EQ(copy->getType(), ElementType::Init);
}
