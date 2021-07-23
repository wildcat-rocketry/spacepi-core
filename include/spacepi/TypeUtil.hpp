#ifndef SPACEPI_CORE_TYPEUTIL_HPP
#define SPACEPI_CORE_TYPEUTIL_HPP

namespace spacepi {
    /**
     * \brief Utilities which can be performed on type information
     *
     * \tparam Target The type on which to query information
     */
    template <typename Target>
    class TypeUtil {
        public:
            TypeUtil() = delete;

        private:
            template <typename Target2>
            class NoRefImpl {
                public:
                    using Type = Target2;
            };

            template <typename Target2>
            class NoRefImpl<Target2 &> {
                public:
                    using Type = Target2;
            };

            template <typename Target2>
            class NoRefImpl<const Target2 &> {
                public:
                    using Type = Target2;
            };

            template <typename Target2>
            class NoRefImpl<Target2 &&> {
                public:
                    using Type = Target2;
            };

            template <typename Target2>
            class NoRefImpl<const Target2 &&> {
                public:
                    using Type = Target2;
            };

        public:
            /**
             * \brief The \c Target type without any reference or \c const modifier
             */
            using NoRef = typename NoRefImpl<Target>::Type;
    };
}

#endif
