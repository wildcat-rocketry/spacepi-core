#ifndef SPACEPI_CORE_MESSAGING_NETWORK_SUBSCRIPTIONID_HPP
#define SPACEPI_CORE_MESSAGING_NETWORK_SUBSCRIPTIONID_HPP

#include <cstddef>
#include <cstdint>
#include <functional>
#include <type_traits>
#include <google/protobuf/message.h>
#include <spacepi/messaging/MessageID.pb.h>

namespace spacepi {
    namespace messaging {
        namespace network {
            class SubscriptionID {
                public:
                    SubscriptionID();
                    SubscriptionID(uint64_t messageID, uint64_t instanceID);

                    template <typename MessageType, typename std::enable_if<std::is_base_of<google::protobuf::Message, MessageType>::value>::type * = nullptr>
                    static SubscriptionID create(uint64_t instanceID) {
                        return SubscriptionID(MessageType::descriptor()->options().GetExtension(MessageID), instanceID);
                    }

                    uint64_t getMessageID() const;
                    uint64_t getInstanceID() const;

                    bool operator ==(const SubscriptionID &other) const;
                    bool operator !=(const SubscriptionID &other) const;
                    bool operator < (const SubscriptionID &other) const;
                    bool operator <=(const SubscriptionID &other) const;
                    bool operator > (const SubscriptionID &other) const;
                    bool operator >=(const SubscriptionID &other) const;

                private:
                    uint64_t messageID;
                    uint64_t instanceID;
            };
        }
    }
}

namespace std {
    template <>
    struct hash<spacepi::messaging::network::SubscriptionID> {
        size_t operator ()(const spacepi::messaging::network::SubscriptionID &id) const;
    };
}

#endif
