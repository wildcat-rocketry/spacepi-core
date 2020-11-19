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
        class GenericSubscription {
            public:
                ~GenericSubscription();

                GenericSubscription(GenericSubscription &) = delete;
                GenericSubscription &operator =(GenericSubscription &) = delete;

                GenericSubscription &operator >>(google::protobuf::Message &message);

                const network::SubscriptionID &getID();

            protected:
                GenericSubscription(detail::ImmovableConnection &conn, const network::SubscriptionID &id);

            private:
                detail::ImmovableConnection &conn;
                network::SubscriptionID id;
        };

        template <typename MessageType, typename std::enable_if<std::is_base_of<google::protobuf::Message, MessageType>::value>::type * = nullptr>
        class Subscription : public GenericSubscription {
            public:
                Subscription(Connection &conn, uint64_t instanceID) : GenericSubscription(*conn.conn, network::SubscriptionID(MessageType::descriptor()->options().GetExtension(spacepi::messaging::MessageID), instanceID)) {
                }

                Subscription &operator >>(MessageType &message) {
                    ((GenericSubscription &) *this) >> message;
                    return *this;
                }
        };
    }
}

#endif
