#include <cstdint>
#include <google/protobuf/message.h>
#include <spacepi/messaging/Connection.hpp>
#include <spacepi/messaging/Subscription.hpp>

using namespace google::protobuf;
using namespace spacepi::messaging;

GenericSubscription::~GenericSubscription() {
    conn->unsubscribe(messageID);
}

GenericSubscription &GenericSubscription::operator >>(Message *message) {
    // TODO
    return *this;
}

GenericSubscription::GenericSubscription(Connection *conn, uint32_t messageID) : conn(conn), messageID(messageID) {
}
