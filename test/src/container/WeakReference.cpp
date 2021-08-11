#include <gtest/gtest.h>
#include <spacepi/container/StrongReference.hpp>
#include <spacepi/container/WeakReference.hpp>
#include <spacepi/test/ElementType.hpp>
#include <spacepi/Parameter.hpp>

using namespace spacepi;
using namespace spacepi::container;
using namespace spacepi::test;

TEST(container_WeakReference, ctors) {
    StrongReference<ElementType> strong;
    ElementType *strongPtr = &(ElementType &) strong;

    WeakReference<ElementType> empty;
    EXPECT_EQ(empty.lock(), nullptr);

    WeakReference<ElementType> weak(strong);
    EXPECT_EQ(weak.lock(), strongPtr);
    weak.unlock();

    const WeakReference<ElementType> &constWeak = weak;
    EXPECT_EQ(constWeak.lock(), (const ElementType *) strongPtr);
    constWeak.unlock();

    WeakReference<ElementType> weakCopy(weak);
    EXPECT_EQ(weakCopy.lock(), strongPtr);
    weakCopy.unlock();

    EXPECT_EQ(weak.lock(), strongPtr);
    weak.unlock();
    WeakReference<ElementType> weakMove(Parameter::move<WeakReference<ElementType>>(weak));
    EXPECT_EQ(weakMove.lock(), strongPtr);
    weakMove.unlock();
    EXPECT_EQ(weak.lock(), nullptr);
}

TEST(container_WeakReference, canBeDeleted) {
    WeakReference<ElementType> weak;
    {
        StrongReference<ElementType> strong;
        weak = strong;
        EXPECT_EQ(weak.lock(), &(ElementType &) strong);
        weak.unlock();
    }
    EXPECT_EQ(weak.lock(), nullptr);
}

TEST(container_WeakReference, losesCopy) {
    StrongReference<ElementType> strong;
    WeakReference<ElementType> weak(strong);
    StrongReference<ElementType> copy(strong);
    EXPECT_EQ(weak.lock(), &(ElementType &) strong);
    weak.unlock();
}

TEST(container_WeakReference, survivesMove) {
    StrongReference<ElementType> strong;
    WeakReference<ElementType> weak(strong);
    StrongReference<ElementType> move(Parameter::move<StrongReference<ElementType>>(strong));
    EXPECT_EQ(weak.lock(), &(ElementType &) move);
    weak.unlock();
}
