#ifndef SPACEPI_CORE_LOG_LOGTARGET_HPP
#define SPACEPI_CORE_LOG_LOGTARGET_HPP

#include <spacepi/log/Entry.hpp>

namespace spacepi {
    namespace log {
        class LogTarget {
            public:
                virtual void operator <<(const Entry &entry) = 0;
        };
    }
}

#endif
