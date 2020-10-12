#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/messaging/network/NetworkThread.hpp>
#include <spacepi/router/PubSubRouter.hpp>
#include <spacepi/router/StreamClient.hpp>
#include <spacepi/router/StreamServer.hpp>
#include <spacepi/util/Exception.hpp>

using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::asio::local;
using namespace spacepi::log;
using namespace spacepi::messaging::network;
using namespace spacepi::router;
using namespace spacepi::util;

template <>
template <>
StreamServer<tcp>::StreamServer<tcp::endpoint>(PubSubRouter *router, const tcp::endpoint &endpoint) : acceptor(NetworkThread::instance.getContext(), endpoint), router(router) {
    startAccept();
}

template <>
template <>
StreamServer<stream_protocol>::StreamServer<stream_protocol::endpoint>(PubSubRouter *router, const stream_protocol::endpoint &endpoint) : acceptor(NetworkThread::instance.getContext(), endpoint), router(router) {
    startAccept();
}

template <typename Proto>
void StreamServer<Proto>::handleAccept() {
    clients.push_back(newClient);
    newClient->sendHello();
    startAccept();
}

template <typename Proto>
void StreamServer<Proto>::handleAcceptError(Exception::pointer err) {
    startAccept();
    handleError(err);
}

template <typename Proto>
void StreamServer<Proto>::handleError(Exception::pointer err) {
    log(LogLevel::Error) << "Socket error: " << err;
}

template <typename Proto>
void StreamServer<Proto>::startAccept() {
    newClient.reset(new StreamClient(router, this));
    newClient->accept(acceptor);
}
