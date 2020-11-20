#include <string>
#include <boost/system/error_code.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/messaging/HelloMessage.pb.h>
#include <spacepi/messaging/MessageID.pb.h>
#include <spacepi/messaging/network/MessagingSocket.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/router/PubSubEndpoint.hpp>
#include <spacepi/router/PubSubRouter.hpp>
#include <spacepi/router/StreamClient.hpp>
#include <spacepi/router/StreamClientCallback.hpp>

using namespace std;
using namespace boost;
using namespace spacepi;
using namespace spacepi::log;
using namespace spacepi::messaging;
using namespace spacepi::messaging::network;
using namespace spacepi::router;
using namespace spacepi::util;

StreamClient::StreamClient(PubSubRouter &router, StreamClientCallback &callback) noexcept : PubSubEndpoint(router), MessagingSocket((MessagingCallback &) *this), callback(callback), accepted(false) {
}

void StreamClient::sendHello() {
    messaging::detail::HelloMessage msg;
    msg.set_serverid("SpacePi Router v2.0\n");
    sendMessage(SubscriptionID(messaging::detail::HelloMessage::descriptor()->options().GetExtension(MessageID), 0), msg.SerializeAsString());
}

void StreamClient::handlePublish(const SubscriptionID &id, const string &data) {
    sendMessage(id, data);
}

void StreamClient::handleMessage(const SubscriptionID &id, const string &data) {
    publish(id, data);
}

void StreamClient::handleAccept() {
    accepted = true;
    log(LogLevel::Debug) << "Client connected.";
    callback.handleAccept(nullptr);
}

void StreamClient::handleError(const Exception::pointer &err) {
    if (accepted) {
        log(LogLevel::Debug) << "Client disconnected.";
    } else {
        callback.handleAccept(&err);
    }
}
