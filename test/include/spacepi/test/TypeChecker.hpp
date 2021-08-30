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
                        static const ReferenceKind Kind;
                };

                template <typename CompType>
                class CompositeCheck {
                    public:
                        static const ReferenceKind Kind;
                };

                TypeChecker() = delete;
        };
    }
}

#endif
