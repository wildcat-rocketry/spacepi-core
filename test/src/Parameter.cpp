#include <gtest/gtest.h>
#include <spacepi/Parameter.hpp>
#include <spacepi/test/TypeChecker.hpp>

using namespace spacepi;
using namespace spacepi::test;

TEST(Parameter, move) {
    int obj;
    EXPECT_EQ(TypeChecker::IntCheck<decltype(Parameter::move<      int   >((      int   ) obj))>::Kind, TypeChecker::ModifiableRValue);
    EXPECT_EQ(TypeChecker::IntCheck<decltype(Parameter::move<      int & >((      int & ) obj))>::Kind, TypeChecker::ModifiableRValue);
    EXPECT_EQ(TypeChecker::IntCheck<decltype(Parameter::move<const int & >((const int & ) obj))>::Kind, TypeChecker::ConstRValue     );
    EXPECT_EQ(TypeChecker::IntCheck<decltype(Parameter::move<      int &&>((      int &&) obj))>::Kind, TypeChecker::ModifiableRValue);
    EXPECT_EQ(TypeChecker::IntCheck<decltype(Parameter::move<const int &&>((const int &&) obj))>::Kind, TypeChecker::ConstRValue     );
}

TEST(Parameter, forward) {
    int obj;
    EXPECT_EQ(TypeChecker::IntCheck<decltype(Parameter::forward<      int   >((      int   ) obj))>::Kind, TypeChecker::ModifiableRValue);
    EXPECT_EQ(TypeChecker::IntCheck<decltype(Parameter::forward<      int & >((      int & ) obj))>::Kind, TypeChecker::ModifiableLValue);
    EXPECT_EQ(TypeChecker::IntCheck<decltype(Parameter::forward<const int & >((const int & ) obj))>::Kind, TypeChecker::ConstLValue     );
    EXPECT_EQ(TypeChecker::IntCheck<decltype(Parameter::forward<      int &&>((      int &&) obj))>::Kind, TypeChecker::ModifiableRValue);
    EXPECT_EQ(TypeChecker::IntCheck<decltype(Parameter::forward<const int &&>((const int &&) obj))>::Kind, TypeChecker::ConstRValue     );
}
