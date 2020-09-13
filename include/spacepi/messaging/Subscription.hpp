#ifndef SPACEPI_CORE_MESSAGING_SUBSCRIPTION_HPP
#define SPACEPI_CORE_MESSAGING_SUBSCRIPTION_HPP

#include <cstdint>
#include <memory>
#include <type_traits>
#include <unordered_set>
#include <google/protobuf/message.h>
#include <spacepi/messaging/message-id.pb.h>

namespace spacepi {
    namespace messaging {
        class Connection;

        class GenericSubscription {
            public:
                GenericSubscription(const GenericSubscription &) = delete;
                virtual ~GenericSubscription();

                GenericSubscription &operator >>(google::protobuf::Message *message);

                uint32_t getMessageID();

            protected:
                GenericSubscription(Connection *conn, uint32_t messageID);

            private:
                Connection *conn;
                uint32_t messageID;
        };

        template <typename MessageType, typename std::enable_if<std::is_base_of<google::protobuf::Message, MessageType>::value>::type * = nullptr>
        class Subscription : public GenericSubscription {
            public:
                Subscription(Connection *conn) : GenericSubscription(conn, messageID) {
                }

                Subscription &operator >>(MessageType &message) {
                    this >> &message;
                    return this;
                }

            private:
                static const uint32_t messageID = MessageType::descriptor()->options().GetExtension(spacepi::messaging::messageID);
        };
    }
}

#endif
