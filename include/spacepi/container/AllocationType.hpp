#ifndef SPACEPI_CONTAINER_ALLOCATIONTYPE_HPP
#define SPACEPI_CONTAINER_ALLOCATIONTYPE_HPP

namespace spacepi {
    namespace container {
        /**
         * \brief Enumerations of methods which can be used to allocate container elements
         */
        enum AllocationType
        {
            /**
             * \brief All elements are statically allocated on the stack
             */
            Static,
            /**
             * \brief All elements are dynamically allocated on the heap
             */
            Dynamic,
            /**
             * \brief All elements are temporaries
             */
            Temporary
        };
    }
}

#endif
