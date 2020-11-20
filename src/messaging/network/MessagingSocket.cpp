#include <string>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <spacepi/messaging/EncapsulatedMessage.pb.h>
#include <spacepi/messaging/network/MessagingSocket.hpp>
#include <spacepi/messaging/network/SocketWrapper.hpp>
#include <spacepi/util/Exception.hpp>
#include <spacepi/util/WeakOrRef.hpp>

using namespace std;
using namespace boost;
using namespace boost::asio::ip;
using namespace boost::asio::local;
using namespace spacepi::messaging;
using namespace spacepi::messaging::detail;
using namespace spacepi::messaging::network;
using namespace spacepi::messaging::network::detail;
using namespace spacepi::util;

template void MessagingSocket::accept<tcp>(tcp::acceptor &acceptor);
template void MessagingSocket::accept<stream_protocol>(stream_protocol::acceptor &acceptor);
template void MessagingSocket::connect<tcp>(const tcp::endpoint &endpoint);
template void MessagingSocket::connect<stream_protocol>(const stream_protocol::endpoint &endpoint);

void MessagingSocketAcceptor::operator ()(const system::error_code &err) {
    if (err) {
        socket->callback.handleError(Exception::createPointer(system::system_error(err)));
    } else {
        socket->callback.handleAccept();
        socket->socket->startRead();
    }
}

MessagingSocketAcceptor::MessagingSocketAcceptor(MessagingSocket &socket) noexcept : socket(socket) {
}

void MessagingSocketConnector::operator ()(const system::error_code &err) {
    if (err) {
        socket->callback.handleError(Exception::createPointer(system::system_error(err)));
    } else {
        socket->callback.handleConnect();
        socket->socket->startRead();
    }
}

MessagingSocketConnector::MessagingSocketConnector(MessagingSocket &socket) noexcept : socket(socket) {
}

MessagingSocket::MessagingSocket(MessagingCallback &callback) noexcept : callback(callback) {
}

template <typename Proto>
void MessagingSocket::accept(typename Proto::acceptor &acceptor) {
    socket.reset(new SocketWrapper<Proto>(WeakOrRef<MessagingSocket>(*this)));
    acceptor.async_accept(((SocketWrapper<Proto> &) *socket).getSocket(), MessagingSocketAcceptor(*this));
}

template <typename Proto>
void MessagingSocket::connect(const typename Proto::endpoint &endpoint) {
    socket.reset(new SocketWrapper<Proto>(WeakOrRef<MessagingSocket>(*this)));
    ((SocketWrapper<Proto> &) *socket).getSocket().async_connect(endpoint, MessagingSocketConnector(*this));
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
    callback.handleMessage(SubscriptionID(cap.messageid(), cap.instanceid()), cap.payload());
}

void MessagingSocket::handleError(const Exception::pointer &err) {
    callback.handleError(err);
}
