#include <cstdint>
#include <string>
#include <google/protobuf/message.h>
#include <spacepi/messaging/Connection.hpp>
#include <spacepi/messaging/Subscription.hpp>

using namespace google::protobuf;
using namespace spacepi::messaging;

GenericSubscription::~GenericSubscription() {
    conn->unsubscribe(this);
}

GenericSubscription &GenericSubscription::operator >>(Message *message) {
    recv(message);
    return *this;
}

uint32_t GenericSubscription::getMessageID() {
    return messageID;
}

GenericSubscription::GenericSubscription(ImmovableConnection *conn, uint32_t messageID) : conn(conn), messageID(messageID) {
    conn->subscribe(this);
}

GenericSubscription::GenericSubscription(Connection *conn, uint32_t messageID) : conn((ImmovableConnection *) *conn), messageID(messageID) {
    this->conn->subscribe(this);
}

void GenericSubscription::recv(Message *message) {
    string str = conn->recieve(this);
    message->ParseFromString(str);
}
