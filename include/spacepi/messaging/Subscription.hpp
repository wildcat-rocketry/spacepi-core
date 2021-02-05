#ifndef SPACEPI_CORE_MESSAGING_SUBSCRIPTION_HPP
#define SPACEPI_CORE_MESSAGING_SUBSCRIPTION_HPP

#include <cstdint>
#include <type_traits>
#include <google/protobuf/message.h>
#include <spacepi/messaging/Connection.hpp>
#include <spacepi/messaging/MessageID.pb.h>
#include <spacepi/messaging/network/SubscriptionID.hpp>

namespace spacepi {
    namespace messaging {
        /**
         * \brief Base class for Subscription which allows polymorphic access
         */
        class GenericSubscription {
            public:
                /**
                 * \brief Cancel the Subscription
                 */
                ~GenericSubscription();

                GenericSubscription(GenericSubscription &) = delete;
                GenericSubscription &operator =(GenericSubscription &) = delete;

                /**
                 * \brief Block until a message is available to be read from the Subscription, then read it
                 * 
                 * \param[out] message The message that will be received
                 * 
                 * \return The Subscription to allow \c >> chaining
                 * 
                 * This method is a cancellation point (see spacepi::concurrent::Interrupt).
                 */
                GenericSubscription &operator >>(google::protobuf::Message &message);

                /**
                 * \brief Get the network::SubscriptionID for this Subscription
                 * 
                 * \return The ID
                 */
                const network::SubscriptionID &getID();

            protected:
                /**
                 * \brief Initialize the GenericSubscription
                 * 
                 * \param[in,out] conn The Connection from which to subscribe to messages
                 * \param[in] id The ID to subscribe to
                 */
                GenericSubscription(detail::ImmovableConnection &conn, const network::SubscriptionID &id);

            private:
                detail::ImmovableConnection &conn;
                network::SubscriptionID id;
        };

        /**
         * \brief The main class which is used to subscribe to messages received by a Connection
         */
        template <typename MessageType, typename std::enable_if<std::is_base_of<google::protobuf::Message, MessageType>::value>::type * = nullptr>
        class Subscription : public GenericSubscription {
            public:
                /**
                 * \brief Initialize a Subscription
                 * 
                 * \param[in,out] conn The Connection from which to subscribe to messages
                 * \param[in] instanceID The instance ID of the messages which are being subscribed to
                 */
                Subscription(Connection &conn, uint64_t instanceID) : GenericSubscription(*conn.conn, network::SubscriptionID(MessageType::descriptor()->options().GetExtension(MessageID), instanceID)) {
                }

                /**
                 * \copydoc GenericSubscription::operator>>
                 */
                Subscription &operator >>(MessageType &message) {
                    ((GenericSubscription &) *this) >> message;
                    return *this;
                }
        };
    }
}

#endif
