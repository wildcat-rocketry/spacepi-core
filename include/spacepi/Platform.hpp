#ifndef SPACEPI_CORE_PLATFORM_HPP
#define SPACEPI_CORE_PLATFORM_HPP

namespace spacepi {
    /**
     * \brief Class which contains platform-specific APIs
     *
     * These APIs are all very small and should mostly just be single calls into a standard library for the platform.
     *
     * Each member in this class is implemented in a platform-specific implementaiton header file
     * (<spacepi/platform/impl/{name}/{member}.hpp>).  The configuration of these files is then set by
     * including each of those files from a platform-specific configuration header file
     * (<spacepi/platform/config/{name}.hpp>).  Which platform-specific configuration file to use is
     * determined by the build configuration.
     */
    class Platform {
        public:
            class Ints;

            Platform() = delete;

            /**
             * \brief Initializes the SpacePi platform
             */
            static inline void platformInit() noexcept;

            /**
             * \brief Prints a message to the console
             *
             * \param[in] msg The message to print
             * \param[in] length The length of the message to print
             */
            static inline void printConsole(const char *msg, int length) noexcept;

            /**
             * \brief Performs an atomic test and set operation
             *
             * This is the atomic equivalent to the following operation:
             * \code{.cpp}
             * static inline bool Platform::testAndSet(volatile int &val) {
             *     int old = val;
             *     val = 1;
             *     return old != 0;
             * }
             * \endcode
             *
             * \param[in,out] val The value to test and set
             * \return The tested value
             *
             * \see unset
             */
            static inline bool testAndSet(volatile int &val) noexcept;

            /**
             * \brief Performs an atomic clear operation
             *
             * This is the atomic equivalent to the following operation:
             * \code{.cpp}
             * static inline void Platform::unset(volatile int &val) {
             *     val = 0;
             * }
             * \endcode
             *
             * \param[in] val The value to clear
             *
             * \see testAndSet
             */
            static inline void unset(volatile int &val) noexcept;
    };
}

#ifndef SPACEPI_HAS_PLATFORM_IMPL

/**
 * \brief Standardized \c int types
 */
class spacepi::Platform::Ints {
    public:
        Ints() = delete;

        /**
         * \brief 8-bit unsigned integer type
         */
        using u8 = unsigned;

        /**
         * \brief 16-bit unsigned integer type
         */
        using u16 = unsigned;

        /**
         * \brief 32-bit unsigned integer type
         */
        using u32 = unsigned;

        /**
         * \brief 64-bit unsigned integer type
         */
        using u64 = unsigned;

        /**
         * \brief 8-bit signed integer type
         */
        using i8 = signed;

        /**
         * \brief 16-bit signed integer type
         */
        using i16 = signed;

        /**
         * \brief 32-bit signed integer type
         */
        using i32 = signed;

        /**
         * \brief 64-bit signed integer type
         */
        using i64 = signed;
};

extern "C" void COMPILER_ERROR_PlatformIsNotSpecified() {
    spacepi::Platform::platformInit();
}

#else
#include <spacepi/platform/current.hpp>
#endif

#endif
