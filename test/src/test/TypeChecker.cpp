#include <spacepi/test/TypeChecker.hpp>

using namespace spacepi::test;

template <>
const TypeChecker::ReferenceKind TypeChecker::IntCheck<int>::Kind = NonReference;

template <>
const TypeChecker::ReferenceKind TypeChecker::IntCheck<int &>::Kind = ModifiableLValue;

template <>
const TypeChecker::ReferenceKind TypeChecker::IntCheck<const int &>::Kind = ConstLValue;

template <>
const TypeChecker::ReferenceKind TypeChecker::IntCheck<int &&>::Kind = ModifiableRValue;

template <>
const TypeChecker::ReferenceKind TypeChecker::IntCheck<const int &&>::Kind = ConstRValue;

template <>
const TypeChecker::ReferenceKind TypeChecker::CompositeCheck<TypeChecker::CompositeType>::Kind = NonReference;

template <>
const TypeChecker::ReferenceKind TypeChecker::CompositeCheck<TypeChecker::CompositeType &>::Kind = ModifiableLValue;

template <>
const TypeChecker::ReferenceKind TypeChecker::CompositeCheck<const TypeChecker::CompositeType &>::Kind = ConstLValue;

template <>
const TypeChecker::ReferenceKind TypeChecker::CompositeCheck<TypeChecker::CompositeType &&>::Kind = ModifiableRValue;

template <>
const TypeChecker::ReferenceKind TypeChecker::CompositeCheck<const TypeChecker::CompositeType &&>::Kind = ConstRValue;
