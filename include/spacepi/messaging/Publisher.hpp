#ifndef SPACEPI_CORE_MESSAGING_PUBLISHER_HPP
#define SPACEPI_CORE_MESSAGING_PUBLISHER_HPP

#include <cstdint>
#include <memory>
#include <google/protobuf/message.h>
#include <spacepi/log/AutoLog.hpp>

namespace spacepi {
    namespace messaging {
        class RawConnection;

        /**
         * \brief Temporary helper class for publishing messages to a Connection
         */
        class Publisher final : private spacepi::log::AutoLog<decltype("core:messaging"_autolog)> {
            friend class RawConnection;

            public:
                /**
                 * \brief Publish a message to all subscribers on different Connection's
                 * 
                 * \param[in] message The message to publish
                 * 
                 * \return The Publisher to allow \c << chaining
                 * 
                 * This method is a cancellation point (see spacepi::concurrent::Interrupt).
                 */
                const Publisher &operator <<(const google::protobuf::Message &message) const;

            private:
                Publisher(const std::shared_ptr<RawConnection> &conn, uint64_t instanceID) noexcept;

                const std::shared_ptr<RawConnection> conn;
                const uint64_t instanceID;
        };
    }
}

#endif
