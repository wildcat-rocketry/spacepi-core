#ifndef SPACEPI_CORE_LOG_CONSOLETARGET_HPP
#define SPACEPI_CORE_LOG_CONSOLETARGET_HPP

#include <ostream>
#include <spacepi/log/Entry.hpp>
#include <spacepi/log/LogTarget.hpp>

namespace spacepi {
    namespace log {
        /**
         * \brief Logging target which prints the log to an \c std::ostream
         */
        class ConsoleTarget : public LogTarget {
            public:
                /**
                 * \brief Creates a new ConsoleTarget which prints to the given \c std::ostream
                 * 
                 * \param[in,out] os The stream to print log entries to
                 */
                explicit ConsoleTarget(std::ostream &os) noexcept;

                void operator <<(const Entry &entry);

            private:
                std::ostream &os;
        };
    }
}

#endif
