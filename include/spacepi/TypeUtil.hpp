#ifndef SPACEPI_CORE_TYPEUTIL_HPP
#define SPACEPI_CORE_TYPEUTIL_HPP

namespace spacepi {
    namespace detail {
        namespace TypeUtil {
            template <typename Target>
            class NoRef {
                public:
                    using Type = Target;
            };

            template <typename Target>
            class NoRef<Target &> {
                public:
                    using Type = Target;
            };

            template <typename Target>
            class NoRef<const Target &> {
                public:
                    using Type = Target;
            };

            template <typename Target>
            class NoRef<Target &&> {
                public:
                    using Type = Target;
            };

            template <typename Target>
            class NoRef<const Target &&> {
                public:
                    using Type = Target;
            };

            template <typename Target>
            class IsPrimitive {
                public:
                    static constexpr bool value = false;
            };

            template <>
            class IsPrimitive<bool> {
                public:
                    static constexpr bool value = true;
            };

            template <>
            class IsPrimitive<char> {
                public:
                    static constexpr bool value = true;
            };

            template <>
            class IsPrimitive<unsigned char> {
                public:
                    static constexpr bool value = true;
            };

            template <>
            class IsPrimitive<signed char> {
                public:
                    static constexpr bool value = true;
            };

            template <>
            class IsPrimitive<unsigned short int> {
                public:
                    static constexpr bool value = true;
            };

            template <>
            class IsPrimitive<signed short int> {
                public:
                    static constexpr bool value = true;
            };

            template <>
            class IsPrimitive<unsigned int> {
                public:
                    static constexpr bool value = true;
            };

            template <>
            class IsPrimitive<signed int> {
                public:
                    static constexpr bool value = true;
            };

            template <>
            class IsPrimitive<unsigned long int> {
                public:
                    static constexpr bool value = true;
            };

            template <>
            class IsPrimitive<signed long int> {
                public:
                    static constexpr bool value = true;
            };

            template <>
            class IsPrimitive<unsigned long long int> {
                public:
                    static constexpr bool value = true;
            };

            template <>
            class IsPrimitive<signed long long int> {
                public:
                    static constexpr bool value = true;
            };

            template <>
            class IsPrimitive<float> {
                public:
                    static constexpr bool value = true;
            };

            template <>
            class IsPrimitive<double> {
                public:
                    static constexpr bool value = true;
            };

            template <>
            class IsPrimitive<long double> {
                public:
                    static constexpr bool value = true;
            };

            template <typename Target>
            class IsPrimitive<Target *> {
                public:
                    static constexpr bool value = true;
            };

            template <typename Target, bool isPrimitive>
            class PrimitiveOrReference {
            };

            template <typename Target>
            class PrimitiveOrReference<Target, false> {
                public:
                    using Type = typename NoRef<Target>::Type &;
            };

            template <typename Target>
            class PrimitiveOrReference<Target, true> {
                public:
                    using Type = typename NoRef<Target>::Type;
            };
        }
    }

    /**
     * \brief Utilities which can be performed on type information
     *
     * \tparam Target The type on which to query information
     */
    template <typename Target>
    class TypeUtil {
        public:
            TypeUtil() = delete;

            /**
             * \brief The \c Target type without any reference or \c const modifier
             */
            using NoRef = typename detail::TypeUtil::NoRef<Target>::Type;

            /**
             * \brief Determines if \c Target is a primitive type
             */
            static constexpr bool isPrimitive() {
                return detail::TypeUtil::IsPrimitive<NoRef>::value;
            }

            /**
             * \brief The \c Target type as a lvalue reference or a primitive non-reference
             */
            using PrimitiveOrReference = typename detail::TypeUtil::PrimitiveOrReference<NoRef, isPrimitive()>::Type;
    };
}

#endif
