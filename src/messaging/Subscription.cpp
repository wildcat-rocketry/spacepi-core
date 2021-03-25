#include <string>
#include <google/protobuf/message.h>
#include <spacepi/messaging/Subscription.hpp>
#include <spacepi/messaging/RawConnection.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>

using namespace google::protobuf;
using namespace spacepi::messaging;
using namespace spacepi::messaging::detail;

GenericSubscription::~GenericSubscription() {
    conn.unsubscribe(*this);
}

GenericSubscription &GenericSubscription::operator >>(Message &message) {
    message.ParseFromString(conn.receive(*this));
    return *this;
}

const network::SubscriptionID &GenericSubscription::getID() {
    return id;
}

GenericSubscription::GenericSubscription(RawConnection &conn, const network::SubscriptionID &id) : conn(conn), id(id) {
    conn.subscribe(*this);
}
