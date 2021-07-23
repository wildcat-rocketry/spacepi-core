#include <gtest/gtest.h>
#include <spacepi/Parameter.hpp>
#include <spacepi/test/TypeChecker.hpp>

using namespace spacepi;
using namespace spacepi::test;

TEST(Parameter, move) {
    int obj;
    ASSERT_EQ(TypeChecker::IntCheck<decltype(Parameter::move<      int   >((      int   ) obj))>::Kind, TypeChecker::ModifiableRValue);
    ASSERT_EQ(TypeChecker::IntCheck<decltype(Parameter::move<      int & >((      int & ) obj))>::Kind, TypeChecker::ModifiableRValue);
    ASSERT_EQ(TypeChecker::IntCheck<decltype(Parameter::move<const int & >((const int & ) obj))>::Kind, TypeChecker::ConstRValue     );
    ASSERT_EQ(TypeChecker::IntCheck<decltype(Parameter::move<      int &&>((      int &&) obj))>::Kind, TypeChecker::ModifiableRValue);
    ASSERT_EQ(TypeChecker::IntCheck<decltype(Parameter::move<const int &&>((const int &&) obj))>::Kind, TypeChecker::ConstRValue     );
}

TEST(Parameter, forward) {
    int obj;
    ASSERT_EQ(TypeChecker::IntCheck<decltype(Parameter::forward<      int   >((      int   ) obj))>::Kind, TypeChecker::ModifiableRValue);
    ASSERT_EQ(TypeChecker::IntCheck<decltype(Parameter::forward<      int & >((      int & ) obj))>::Kind, TypeChecker::ModifiableLValue);
    ASSERT_EQ(TypeChecker::IntCheck<decltype(Parameter::forward<const int & >((const int & ) obj))>::Kind, TypeChecker::ConstLValue     );
    ASSERT_EQ(TypeChecker::IntCheck<decltype(Parameter::forward<      int &&>((      int &&) obj))>::Kind, TypeChecker::ModifiableRValue);
    ASSERT_EQ(TypeChecker::IntCheck<decltype(Parameter::forward<const int &&>((const int &&) obj))>::Kind, TypeChecker::ConstRValue     );
}
