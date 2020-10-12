#ifndef SPACEPI_CORE_UTIL_FUNCTIONAL_HPP
#define SPACEPI_CORE_UTIL_FUNCTIONAL_HPP

namespace spacepi {
    namespace util {
        template <typename ToType, typename FromType>
        class Cast {
            public:
                ToType operator ()(FromType x) const noexcept {
                    return (ToType) x;
                }
        };
    }
}

#endif
