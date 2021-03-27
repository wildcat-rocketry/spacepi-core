#ifndef SPACEPI_CORE_MESSAGING_CONNECTION_HPP
#define SPACEPI_CORE_MESSAGING_CONNECTION_HPP

#include <cstdint>
#include <memory>
#include <spacepi/messaging/Publisher.hpp>
#include <spacepi/util/Command.hpp>

namespace spacepi {
    namespace messaging {
        class RawConnection;

        /**
         * \brief The main class which is used to handle a connection to the router, and therefore other modules
         */
        class Connection final {
            public:
                /**
                 * \brief Initialize a Connection
                 * 
                 * \param[in] cmd The command to use to configure the Connection
                 */
                explicit Connection(spacepi::util::Command &cmd);

                /**
                 * \brief Starts a publish operation for the given instance ID
                 * 
                 * \param[in] instanceID The instance ID
                 * 
                 * \return The publishing helper (see Publisher::operator<<)
                 */
                Publisher operator ()(uint64_t instanceID) const noexcept;

                /**
                 * \brief Gets the RawConnection associated with this Connection
                 * 
                 * \return The RawConnection
                 */
                const std::shared_ptr<RawConnection> &raw() const noexcept;

            private:
                const std::shared_ptr<RawConnection> conn;
        };
    }
}

#endif
