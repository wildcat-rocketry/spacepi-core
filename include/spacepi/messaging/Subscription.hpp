#ifndef SPACEPI_CORE_MESSAGING_SUBSCRIPTION_HPP
#define SPACEPI_CORE_MESSAGING_SUBSCRIPTION_HPP

#include <cstdint>
#include <type_traits>
#include <google/protobuf/message.h>
#include <spacepi/messaging/MessageID.pb.h>

namespace spacepi {
    namespace messaging {
        class Connection;
        class ImmovableConnection;

        class GenericSubscription {
            public:
                GenericSubscription(const GenericSubscription &) = delete;
                virtual ~GenericSubscription();

                GenericSubscription &operator >>(google::protobuf::Message *message);

                uint32_t getMessageID();

            protected:
                GenericSubscription(ImmovableConnection *conn, uint32_t messageID);
                GenericSubscription(Connection *conn, uint32_t messageID);

                void recv(google::protobuf::Message *message);

            private:
                ImmovableConnection *conn;
                uint32_t messageID;
        };

        template <typename MessageType, typename std::enable_if<std::is_base_of<google::protobuf::Message, MessageType>::value>::type * = nullptr>
        class Subscription : public GenericSubscription {
            public:
                Subscription(ImmovableConnection *conn) : GenericSubscription(conn, MessageType::descriptor()->options().GetExtension(spacepi::messaging::MessageID)) {
                }

                Subscription(Connection *conn) : GenericSubscription(conn, MessageType::descriptor()->options().GetExtension(spacepi::messaging::MessageID)) {
                }

                Subscription &operator >>(MessageType &message) {
                    recv(&message);
                    return *this;
                }
        };
    }
}

#endif
