#include <string>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/messaging/EncapsulatedMessage.pb.h>
#include <spacepi/messaging/network/MessagingSocket.hpp>
#include <spacepi/messaging/network/SocketWrapper.hpp>
#include <spacepi/util/Exception.hpp>

using namespace std;
using namespace boost;
using namespace boost::asio::ip;
using namespace boost::asio::local;
using namespace spacepi::log;
using namespace spacepi::messaging;
using namespace spacepi::messaging::network;
using namespace spacepi::util;

void MessagingCallback::handleAccept() {
}

void MessagingCallback::handleConnect() {
}

void DefaultMessagingCallback::handleError(Exception::pointer err) {
    log(LogLevel::Error) << "MessagingSocket unhandled error: " << err;
}

void MessagingSocketAcceptor::operator ()(const system::error_code &err) {
    if (err) {
        socket->callback->handleError(Exception::createPointer(EXCEPTION(MessagingException("Error while accepting socket")) << errinfo_nested_exception(Exception::createPointer(system::system_error(err)))));
    } else {
        socket->callback->handleAccept();
        socket->socket->startRead();
    }
}

MessagingSocketAcceptor::MessagingSocketAcceptor(MessagingSocket *socket) : socket(socket) {
}

void MessagingSocketConnector::operator ()(const system::error_code &err) {
    if (err) {
        socket->callback->handleError(Exception::createPointer(EXCEPTION(MessagingException("Error while connecting socket")) << errinfo_nested_exception(Exception::createPointer(system::system_error(err)))));
    } else {
        socket->callback->handleConnect();
        socket->socket->startRead();
    }
}

MessagingSocketConnector::MessagingSocketConnector(MessagingSocket *socket) : socket(socket) {
}

MessagingSocket::MessagingSocket(MessagingCallback *callback) : callback(callback), acceptor(this), connector(this) {
}

template <>
void MessagingSocket::accept<tcp::acceptor>(tcp::acceptor &acceptor) {
    std::shared_ptr<SocketWrapper<tcp>> s(new SocketWrapper<tcp>(this));
    acceptor.async_accept(s->getSocket(), this->acceptor);
    socket = static_pointer_cast<GenericSocketWrapper>(s);
}

template <>
void MessagingSocket::accept<stream_protocol::acceptor>(stream_protocol::acceptor &acceptor) {
    std::shared_ptr<SocketWrapper<stream_protocol>> s(new SocketWrapper<stream_protocol>(this));
    acceptor.async_accept(s->getSocket(), this->acceptor);
    socket = static_pointer_cast<GenericSocketWrapper>(s);
}

template <>
void MessagingSocket::connect<tcp::endpoint>(const tcp::endpoint &endpoint) {
    std::shared_ptr<SocketWrapper<tcp>> s(new SocketWrapper<tcp>(this));
    s->getSocket().async_connect(endpoint, this->connector);
    socket = static_pointer_cast<GenericSocketWrapper>(s);
}

template <>
void MessagingSocket::connect<stream_protocol::endpoint>(const stream_protocol::endpoint &endpoint) {
    std::shared_ptr<SocketWrapper<stream_protocol>> s(new SocketWrapper<stream_protocol>(this));
    s->getSocket().async_connect(endpoint, this->connector);
    socket = static_pointer_cast<GenericSocketWrapper>(s);
}

void MessagingSocket::sendMessage(const SubscriptionID &id, const std::string &msg) {
    EncapsulatedMessage cap;
    cap.set_messageid(id.getMessageID());
    cap.set_instanceid(id.getInstanceID());
    cap.set_payload(msg);
    socket->sendPacket(cap.SerializeAsString());
}

void MessagingSocket::handlePacket(const std::string &pkt) {
    EncapsulatedMessage cap;
    cap.ParseFromString(pkt);
    callback->handleMessage(SubscriptionID(cap.messageid(), cap.instanceid()), cap.payload());
}

void MessagingSocket::handleError(Exception::pointer err) {
    callback->handleError(err);
}
