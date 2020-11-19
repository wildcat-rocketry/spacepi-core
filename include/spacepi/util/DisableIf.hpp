#ifndef SPACEPI_CORE_UTIL_DISABLEIF_HPP
#define SPACEPI_CORE_UTIL_DISABLEIF_HPP

namespace spacepi {
    namespace util {
        template <bool Cond, typename Type = void>
        struct DisableIf {
        };

        template <typename Type>
        struct DisableIf<false, Type> {
            typedef Type type;
        };
    }
}

#endif
