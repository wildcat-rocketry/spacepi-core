#ifndef SPACEPI_CORE_TEST_TYPECHECKER_HPP
#define SPACEPI_CORE_TEST_TYPECHECKER_HPP

namespace spacepi {
    namespace test {
        class TypeChecker {
            public:
                enum ReferenceKind {
                    Invalid,
                    NonReference,
                    ModifiableLValue,
                    ConstLValue,
                    ModifiableRValue,
                    ConstRValue
                };

                class CompositeType {
                };

                template <typename IntType>
                class IntCheck {
                    public:
                        static constexpr ReferenceKind Kind = Invalid;
                };

                template <typename CompType>
                class CompositeCheck {
                    public:
                        static constexpr ReferenceKind Kind = Invalid;
                };

                TypeChecker() = delete;
        };

        template <>
        class TypeChecker::IntCheck<int> {
            public:
                static constexpr ReferenceKind Kind = NonReference;
        };

        template <>
        class TypeChecker::IntCheck<int &> {
            public:
                static constexpr ReferenceKind Kind = ModifiableLValue;
        };

        template <>
        class TypeChecker::IntCheck<const int &> {
            public:
                static constexpr ReferenceKind Kind = ConstLValue;
        };

        template <>
        class TypeChecker::IntCheck<int &&> {
            public:
                static constexpr ReferenceKind Kind = ModifiableRValue;
        };

        template <>
        class TypeChecker::IntCheck<const int &&> {
            public:
                static constexpr ReferenceKind Kind = ConstRValue;
        };

        template <>
        class TypeChecker::CompositeCheck<TypeChecker::CompositeType> {
            public:
                static constexpr ReferenceKind Kind = NonReference;
        };

        template <>
        class TypeChecker::CompositeCheck<TypeChecker::CompositeType &> {
            public:
                static constexpr ReferenceKind Kind = ModifiableLValue;
        };

        template <>
        class TypeChecker::CompositeCheck<const TypeChecker::CompositeType &> {
            public:
                static constexpr ReferenceKind Kind = ConstLValue;
        };

        template <>
        class TypeChecker::CompositeCheck<TypeChecker::CompositeType &&> {
            public:
                static constexpr ReferenceKind Kind = ModifiableRValue;
        };

        template <>
        class TypeChecker::CompositeCheck<const TypeChecker::CompositeType &&> {
            public:
                static constexpr ReferenceKind Kind = ConstRValue;
        };
    }
}

#endif
