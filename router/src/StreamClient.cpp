#include <cstdint>
#include <string>
#include <boost/system/error_code.hpp>
#include <spacepi/messaging/HelloMessage.pb.h>
#include <spacepi/messaging/MessageID.pb.h>
#include <spacepi/messaging/network/MessagingSocket.hpp>
#include <spacepi/router/StreamClient.hpp>
#include <spacepi/router/PubSubEndpoint.hpp>
#include <spacepi/router/PubSubRouter.hpp>

using namespace std;
using namespace boost;
using namespace spacepi::messaging;
using namespace spacepi::messaging::network;
using namespace spacepi::router;
using namespace spacepi::util;

StreamClient::StreamClient(PubSubRouter *router, StreamClientCallback *callback) : PubSubEndpoint(router), MessagingSocket(this), callback(callback), accepted(false) {
}

void StreamClient::sendHello() {
    HelloMessage msg;
    msg.set_serverid("SpacePi Router v2.0\n");
    sendMessage(HelloMessage::descriptor()->options().GetExtension(MessageID), msg.SerializeAsString());
}

void StreamClient::handlePublish(uint32_t id, const string &data) {
    sendMessage(id, data);
}

void StreamClient::handleMessage(uint32_t id, const string &data) {
    publish(id, data);
}

void StreamClient::handleAccept() {
    accepted = true;
    callback->handleAccept();
}

void StreamClient::handleError(Exception::pointer err) {
    if (accepted) {
        callback->handleError(err);
    } else {
        callback->handleAcceptError(err);
    }
}
