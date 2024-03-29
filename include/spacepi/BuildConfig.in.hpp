#ifndef SPACEPI_CORE_CONFIG_HPP
#define SPACEPI_CORE_CONFIG_HPP

namespace spacepi {
    /**
     * \brief Settings captured from the CMake build configuration
     */
    class BuildConfig {
        public:
            BuildConfig() = delete;

            /**
             * \brief The binary directory to which the core repo builds
             */
            static constexpr const char *CMAKE_CURRENT_BINARY_DIR() {
                return "${CMAKE_CURRENT_BINARY_DIR}";
            }

            /**
             * \brief The number of characters in CMAKE_CURRENT_BINARY_DIR
             */
            static constexpr decltype(sizeof(int)) CMAKE_CURRENT_BINARY_DIR_len() {
                return sizeof("${CMAKE_CURRENT_BINARY_DIR}") - 1;
            }
    };
}

#endif
