#ifndef SPACEPI_CORE_MESSAGING_SUBSCRIPTION_HPP
#define SPACEPI_CORE_MESSAGING_SUBSCRIPTION_HPP

#include <cstdint>
#include <type_traits>
#include <google/protobuf/message.h>
#include <spacepi/messaging/MessageID.pb.h>
#include <spacepi/messaging/network/SubscriptionID.hpp>

namespace spacepi {
    namespace messaging {
        class Connection;
        class ImmovableConnection;

        class GenericSubscription {
            public:
                GenericSubscription(const GenericSubscription &) = delete;
                virtual ~GenericSubscription();

                GenericSubscription &operator >>(google::protobuf::Message *message);

                const network::SubscriptionID &getID();

            protected:
                GenericSubscription(ImmovableConnection *conn, const network::SubscriptionID &id);
                GenericSubscription(Connection *conn, const network::SubscriptionID &id);

                void recv(google::protobuf::Message *message);

            private:
                ImmovableConnection *conn;
                network::SubscriptionID id;
        };

        template <typename MessageType, typename std::enable_if<std::is_base_of<google::protobuf::Message, MessageType>::value>::type * = nullptr>
        class Subscription : public GenericSubscription {
            public:
                Subscription(ImmovableConnection *conn, uint32_t instanceID) : GenericSubscription(conn, network::SubscriptionID(MessageType::descriptor()->options().GetExtension(spacepi::messaging::MessageID), instanceID)) {
                }

                Subscription(Connection *conn, uint32_t instanceID) : GenericSubscription(conn, network::SubscriptionID(MessageType::descriptor()->options().GetExtension(spacepi::messaging::MessageID), instanceID)) {
                }

                Subscription &operator >>(MessageType &message) {
                    recv(&message);
                    return *this;
                }
        };
    }
}

#endif
