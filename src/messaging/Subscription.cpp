#include <string>
#include <google/protobuf/message.h>
#include <spacepi/messaging/Connection.hpp>
#include <spacepi/messaging/Subscription.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>

using namespace google::protobuf;
using namespace spacepi::messaging;
using namespace spacepi::messaging::detail;

GenericSubscription::~GenericSubscription() {
    conn.unsubscribe(*this);
}

GenericSubscription &GenericSubscription::operator >>(Message &message) {
    message.ParseFromString(conn.recieve(*this));
    return *this;
}

const network::SubscriptionID &GenericSubscription::getID() {
    return id;
}

GenericSubscription::GenericSubscription(ImmovableConnection &conn, const network::SubscriptionID &id) : conn(conn), id(id) {
    conn.subscribe(*this);
}
