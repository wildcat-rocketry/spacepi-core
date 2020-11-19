#include <string>
#include <google/protobuf/message.h>
#include <spacepi/messaging/Connection.hpp>
#include <spacepi/messaging/Subscription.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>

using namespace google::protobuf;
using namespace spacepi::messaging;
using namespace spacepi::messaging::detail;
using namespace spacepi::messaging::network;

GenericSubscription::~GenericSubscription() {
    conn.unsubscribe(*this);
}

GenericSubscription &GenericSubscription::operator >>(Message &message) {
    message.ParseFromString(conn.recieve(*this));
    return *this;
}

const SubscriptionID &GenericSubscription::getID() {
    return id;
}

GenericSubscription::GenericSubscription(ImmovableConnection &conn, const SubscriptionID &id) : conn(conn), id(id) {
    conn.subscribe(*this);
}
