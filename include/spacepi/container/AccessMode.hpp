#ifndef SPACEPI_CORE_CONTAINER_ACCESSMODE_HPP
#define SPACEPI_CORE_CONTAINER_ACCESSMODE_HPP

namespace spacepi {
    namespace container {
        /**
         * \brief Enumerations of access modes for a Property
         */
        enum AccessMode {
            /**
             * \brief The Property can be read from
             */
            Read,
            /**
             * \brief The Property can be written to
             */
            Write,
            /**
             * \brief The Property can both be read from and written to
             */
            ReadWrite
        };
    }
}

#endif
