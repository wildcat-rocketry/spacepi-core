#include <string>
#include <google/protobuf/message.h>
#include <spacepi/messaging/Connection.hpp>
#include <spacepi/messaging/Subscription.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>

using namespace google::protobuf;
using namespace spacepi::messaging;
using namespace spacepi::messaging::network;

GenericSubscription::~GenericSubscription() {
    conn->unsubscribe(this);
}

GenericSubscription &GenericSubscription::operator >>(Message *message) {
    recv(message);
    return *this;
}

const SubscriptionID &GenericSubscription::getID() {
    return id;
}

GenericSubscription::GenericSubscription(ImmovableConnection *conn, const SubscriptionID &id) : conn(conn), id(id) {
    conn->subscribe(this);
}

GenericSubscription::GenericSubscription(Connection *conn, const SubscriptionID &id) : conn((ImmovableConnection *) *conn), id(id) {
    this->conn->subscribe(this);
}

void GenericSubscription::recv(Message *message) {
    string str = conn->recieve(this);
    message->ParseFromString(str);
}
