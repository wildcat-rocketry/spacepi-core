#ifndef SPACEPI_CORE_LOG_LOGTARGET_HPP
#define SPACEPI_CORE_LOG_LOGTARGET_HPP

#include <spacepi/log/Entry.hpp>

namespace spacepi {
    namespace log {
        /**
         * \brief Base class for classes which can receive logging events
         */
        class LogTarget {
            public:
                /**
                 * \brief Destroy this LogTarget
                 */
                virtual ~LogTarget() = default;

                /**
                 * \brief Writes a new Entry to the log
                 * 
                 * \param[in] entry The new logging Entry
                 */
                virtual void operator <<(const Entry &entry) = 0;
        };
    }
}

#endif
