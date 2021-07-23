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

                template <>
                class IntCheck<int> {
                    public:
                        IntCheck() = delete;

                        static constexpr ReferenceKind Kind = NonReference;
                };

                template <>
                class IntCheck<int &> {
                    public:
                        IntCheck() = delete;

                        static constexpr ReferenceKind Kind = ModifiableLValue;
                };

                template <>
                class IntCheck<const int &> {
                    public:
                        IntCheck() = delete;

                        static constexpr ReferenceKind Kind = ConstLValue;
                };

                template <>
                class IntCheck<int &&> {
                    public:
                        IntCheck() = delete;

                        static constexpr ReferenceKind Kind = ModifiableRValue;
                };

                template <>
                class IntCheck<const int &&> {
                    public:
                        IntCheck() = delete;

                        static constexpr ReferenceKind Kind = ConstRValue;
                };

                TypeChecker() = delete;
        };
    }
}

#endif
