#ifndef SPACEPI_CORE_PLATFORM_INIT_HPP
#define SPACEPI_CORE_PLATFORM_INIT_HPP

namespace spacepi {
    namespace platform {
        /**
         * \brief Initializes the SpacePi platform
         */
        class Init {
            public:
                Init() = delete;

                /**
                 * \brief Initializes the SpacePi platform
                 *
                 * This is called from the program entry point automatically for SpacePi programs.  However, if a custom entry
                 * point is used, this must be called to start any included modules.
                 */
                void platformInit();
        };
    }
}

#endif
