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

                template <typename IntType>
                class IntCheck {
                    public:
                        IntCheck() = delete;

                        static constexpr ReferenceKind Kind = Invalid;
                };

                TypeChecker() = delete;
        };

        template <>
        class TypeChecker::IntCheck<int> {
            public:
                IntCheck() = delete;

                static constexpr ReferenceKind Kind = NonReference;
        };

        template <>
        class TypeChecker::IntCheck<int &> {
            public:
                IntCheck() = delete;

                static constexpr ReferenceKind Kind = ModifiableLValue;
        };

        template <>
        class TypeChecker::IntCheck<const int &> {
            public:
                IntCheck() = delete;

                static constexpr ReferenceKind Kind = ConstLValue;
        };

        template <>
        class TypeChecker::IntCheck<int &&> {
            public:
                IntCheck() = delete;

                static constexpr ReferenceKind Kind = ModifiableRValue;
        };

        template <>
        class TypeChecker::IntCheck<const int &&> {
            public:
                IntCheck() = delete;

                static constexpr ReferenceKind Kind = ConstRValue;
        };
    }
}

#endif
