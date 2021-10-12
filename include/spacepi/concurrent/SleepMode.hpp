#ifndef SPACEPI_CORE_CONCURRENT_SLEEPMODE_HPP
#define SPACEPI_CORE_CONCURRENT_SLEEPMODE_HPP

namespace spacepi {
    namespace concurrent {
        /**
         * \brief Type of algorithms used to sleep while waiting for synchronization
         */
        enum SleepMode
        {
            /**
             * \brief Critical sections are small, so never sleep
             */
            Fast
        };
    }
}

#endif
