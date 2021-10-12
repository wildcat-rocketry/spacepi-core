#ifndef SPACEPI_CORE_INTS_HPP
#define SPACEPI_CORE_INTS_HPP

#include <spacepi/Platform.hpp>

namespace spacepi {
#ifdef BUILDING_DOCS
    /**
     * \brief 8-bit unsigned integer type
     */
    class u8 {
    };

    /**
     * \brief 16-bit unsigned integer type
     */
    class u16 {
    };

    /**
     * \brief 32-bit unsigned integer type
     */
    class u32 {
    };

    /**
     * \brief 64-bit unsigned integer type
     */
    class u64 {
    };

    /**
     * \brief 8-bit signed integer type
     */
    class i8 {
    };

    /**
     * \brief 16-bit signed integer type
     */
    class i16 {
    };

    /**
     * \brief 32-bit signed integer type
     */
    class i32 {
    };

    /**
     * \brief 64-bit signed integer type
     */
    class i64 {
    };
#else
    using u8 = spacepi::Platform::Ints::u8;
    using u16 = spacepi::Platform::Ints::u16;
    using u32 = spacepi::Platform::Ints::u32;
    using u64 = spacepi::Platform::Ints::u64;
    using i8 = spacepi::Platform::Ints::i8;
    using i16 = spacepi::Platform::Ints::i16;
    using i32 = spacepi::Platform::Ints::i32;
    using i64 = spacepi::Platform::Ints::i64;
#endif
}

#endif
