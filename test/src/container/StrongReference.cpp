#include <gtest/gtest.h>
#include <spacepi/container/StrongReference.hpp>
#include <spacepi/test/ElementType.hpp>
#include <spacepi/Parameter.hpp>

using namespace spacepi;
using namespace spacepi::container;
using namespace spacepi::test;

TEST(container_StrongReference, ctors) {
    {
        StrongReference<ElementType> ref(42, ElementType::Tag);
        EXPECT_EQ(ref->getValue(), 42);
        EXPECT_EQ(ref->getType(), ElementType::Tagged);
    }
    {
        StrongReference<ElementType> ref(43);
        EXPECT_EQ(ref->getValue(), 43);
        EXPECT_EQ(ref->getType(), ElementType::Init);
    }
    {
        ElementType orig(44);
        StrongReference<ElementType> ref(orig);
        EXPECT_EQ(ref->getValue(), 44);
        EXPECT_EQ(ref->getType(), ElementType::Copy);
        EXPECT_EQ(orig.getType(), ElementType::Init);
    }
    {
        StrongReference<ElementType> orig(45);
        StrongReference<ElementType> ref(orig);
        EXPECT_EQ(ref->getValue(), 45);
        EXPECT_EQ(ref->getType(), ElementType::Copy);
        EXPECT_EQ(orig->getType(), ElementType::Init);
    }
    {
        ElementType orig(46);
        StrongReference<ElementType> ref(Parameter::move<ElementType>(orig));
        EXPECT_EQ(ref->getValue(), 46);
        EXPECT_EQ(ref->getType(), ElementType::Move);
        EXPECT_EQ(orig.getType(), ElementType::Moved);
    }
    {
        StrongReference<ElementType> orig(47);
        StrongReference<ElementType> ref(Parameter::move<StrongReference<ElementType>>(orig));
        EXPECT_EQ(ref->getValue(), 47);
        EXPECT_EQ(ref->getType(), ElementType::Move);
        EXPECT_EQ(orig->getType(), ElementType::Moved);
    }
    {
        StrongReference<ElementType> ref;
        ref = (const ElementType &) ElementType(48);
        EXPECT_EQ(ref->getValue(), 48);
        EXPECT_EQ(ref->getType(), ElementType::Copy);
    }
    {
        StrongReference<ElementType> ref;
        ref = (const StrongReference<ElementType> &) StrongReference<ElementType>(49);
        EXPECT_EQ(ref->getValue(), 49);
        EXPECT_EQ(ref->getType(), ElementType::Copy);
    }
    {
        StrongReference<ElementType> ref;
        ref = Parameter::move<ElementType>(ElementType(50));
        EXPECT_EQ(ref->getValue(), 50);
        EXPECT_EQ(ref->getType(), ElementType::Move);
    }
    {
        StrongReference<ElementType> ref;
        ref = Parameter::move<StrongReference<ElementType>>(StrongReference<ElementType>(51));
        EXPECT_EQ(ref->getValue(), 51);
        EXPECT_EQ(ref->getType(), ElementType::Move);
    }
}

TEST(container_StrongReference, getters) {
    StrongReference<ElementType> ref;
    const StrongReference<ElementType> &constRef = ref;
    ElementType &modLval = ref;
    const ElementType &constLval = constRef;
    ElementType *modPtr = ref.operator ->();
    const ElementType *constPtr = constRef.operator ->();
    EXPECT_EQ(&modLval, modPtr);
    EXPECT_EQ(&constLval, constPtr);
    EXPECT_EQ((const ElementType *) modPtr, constPtr);
}
