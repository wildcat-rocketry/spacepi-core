#include <gtest/gtest.h>
#include <spacepi/container/SharedReference.hpp>
#include <spacepi/container/WeakReference.hpp>
#include <spacepi/test/ElementType.hpp>
#include <spacepi/Parameter.hpp>

using namespace spacepi;
using namespace spacepi::container;
using namespace spacepi::test;

TEST(container_SharedReference, ctors) {
    SharedReference<ElementType> strong(42, ElementType::Tag);
    ElementType *strongPtr = strong.lock();
    strong.unlock();
    EXPECT_EQ(strongPtr->getValue(), 42);
    EXPECT_EQ(strongPtr->getType(), ElementType::Tagged);

    SharedReference<ElementType> moved(Parameter::move<SharedReference<ElementType>>(strong));
    ElementType *movedPtr = moved.lock();
    moved.unlock();
    EXPECT_NE(movedPtr, strongPtr);
    EXPECT_EQ(movedPtr->getValue(), 42);
    EXPECT_EQ(movedPtr->getType(), ElementType::Move);

    SharedReference<ElementType> copied(moved);
    ElementType *copiedPtr = copied.lock();
    copied.unlock();
    EXPECT_EQ(copiedPtr, movedPtr);
}

TEST(container_SharedReference, weakTracking) {
    SharedReference<ElementType> orig;
    ElementType *origPtr = orig.lock();
    orig.unlock();

    WeakReference<ElementType> weak(orig);
    EXPECT_EQ(weak.lock(), origPtr);
    weak.unlock();

    SharedReference<ElementType> moved(Parameter::move<SharedReference<ElementType>>(orig));
    ElementType *movedPtr = moved.lock();
    moved.unlock();

    EXPECT_EQ(weak.lock(), movedPtr);
    weak.unlock();
}
