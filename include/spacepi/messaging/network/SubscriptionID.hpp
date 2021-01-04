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
            /**
             * \brief Represents an ID which contains both the format of the encoded data and an instance for which the
             * packet is intended
             */
            class SubscriptionID {
                public:
                    /**
                     * \brief Initialize an invalid SubscriptionID
                     */
                    SubscriptionID();

                    /**
                     * \brief Initialize a SubscriptionID
                     * 
                     * \param[in] messageID The ID which describes the format of the encoded data
                     * \param[in] instanceID The ID which describes the instance for which the packet is intended
                     */
                    SubscriptionID(uint64_t messageID, uint64_t instanceID);

                    /**
                     * \brief Create a SubscriptionID for a given protobuf message type and instance ID
                     * 
                     * \param[in] instanceID The ID which describes the instance for which the packet is intended
                     * \tparam MessageType The type of protobuf message which the messageID should describe
                     */
                    template <typename MessageType, typename std::enable_if<std::is_base_of<google::protobuf::Message, MessageType>::value>::type * = nullptr>
                    static SubscriptionID create(uint64_t instanceID) {
                        return SubscriptionID(MessageType::descriptor()->options().GetExtension(MessageID), instanceID);
                    }

                    /**
                     * \brief Gets the message ID from this SubscriptionID
                     * 
                     * \return The message ID
                     */
                    uint64_t getMessageID() const;

                    /**
                     * \brief Gets the instance ID from this SubscriptionID
                     * 
                     * \return The instance ID
                     */
                    uint64_t getInstanceID() const;

                    /**
                     * \brief Compare one SubscriptionID against another
                     * 
                     * \param[in] other The RHS to compare against
                     * 
                     * \return The result of the comparison
                     */
                    bool operator ==(const SubscriptionID &other) const;

                    /**
                     * \copydoc operator==
                     */
                    bool operator !=(const SubscriptionID &other) const;

                    /**
                     * \copydoc operator==
                     */
                    bool operator < (const SubscriptionID &other) const;

                    /**
                     * \copydoc operator==
                     */
                    bool operator <=(const SubscriptionID &other) const;

                    /**
                     * \copydoc operator==
                     */
                    bool operator > (const SubscriptionID &other) const;

                    /**
                     * \copydoc operator==
                     */
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
