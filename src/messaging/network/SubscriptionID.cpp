#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <spacepi/messaging/network/SubscriptionID.hpp>

using namespace std;
using namespace spacepi::messaging::network;

SubscriptionID::SubscriptionID() : messageID(numeric_limits<uint64_t>::max()), instanceID(numeric_limits<uint64_t>::max()) {
}

SubscriptionID::SubscriptionID(uint64_t messageID, uint64_t instanceID) : messageID(messageID), instanceID(instanceID) {
}

uint64_t SubscriptionID::getMessageID() const {
    return messageID;
}

uint64_t SubscriptionID::getInstanceID() const {
    return instanceID;
}

bool SubscriptionID::operator ==(const SubscriptionID &other) const {
    return messageID == other.messageID && instanceID == other.instanceID;
}

bool SubscriptionID::operator !=(const SubscriptionID &other) const {
    return messageID != other.messageID || instanceID != other.instanceID;
}

bool SubscriptionID::operator <(const SubscriptionID &other) const {
    if (messageID == other.messageID) {
        return instanceID < other.instanceID;
    } else {
        return messageID < other.messageID;
    }
}

bool SubscriptionID::operator <=(const SubscriptionID &other) const {
    if (messageID == other.messageID) {
        return instanceID <= other.instanceID;
    } else {
        return messageID <= other.messageID;
    }
}

bool SubscriptionID::operator >(const SubscriptionID &other) const {
    if (messageID == other.messageID) {
        return instanceID > other.instanceID;
    } else {
        return messageID > other.messageID;
    }
}

bool SubscriptionID::operator >=(const SubscriptionID &other) const {
    if (messageID == other.messageID) {
        return instanceID >= other.instanceID;
    } else {
        return messageID >= other.messageID;
    }
}

size_t std::hash<SubscriptionID>::operator ()(const SubscriptionID &id) const {
    union {
        uint64_t data[2];
        size_t result[sizeof(data) / sizeof(size_t)];
    } conv;
    conv.data[0] = id.getMessageID();
    conv.data[1] = id.getInstanceID();
    size_t accum = 0;
    for (int i = 0; i < sizeof(conv.result) / sizeof(conv.result[0]); ++i) {
        accum ^= conv.result[i];
    }
    return accum;
}
