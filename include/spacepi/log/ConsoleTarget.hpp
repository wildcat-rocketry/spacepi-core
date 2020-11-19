#ifndef SPACEPI_CORE_LOG_CONSOLETARGET_HPP
#define SPACEPI_CORE_LOG_CONSOLETARGET_HPP

#include <ostream>
#include <spacepi/log/Entry.hpp>
#include <spacepi/log/LogTarget.hpp>

namespace spacepi {
    namespace log {
        class ConsoleTarget : public LogTarget {
            public:
                explicit ConsoleTarget(std::ostream &os) noexcept;

                void operator <<(const Entry &entry);

            private:
                std::ostream &os;
        };
    }
}

#endif
