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

template class StreamServer<tcp>;
template class StreamServer<stream_protocol>;

template <typename Proto>
StreamServer<Proto>::StreamServer(PubSubRouter &router, const typename Proto::endpoint &endpoint) : acceptor(NetworkThread::instance.getContext(), endpoint), router(router), client(nullptr) {
    startAccept();
}

template <typename Proto>
void StreamServer<Proto>::handleAccept() {
    client->sendHello();
    startAccept();
}

template <typename Proto>
void StreamServer<Proto>::handleAcceptError(const Exception::pointer &err) {
    startAccept();
    handleError(err);
}

template <typename Proto>
void StreamServer<Proto>::handleError(const Exception::pointer &err) {
    log(LogLevel::Error) << "Socket error: " << err;
}

template <typename Proto>
void StreamServer<Proto>::startAccept() {
    client.reset(new StreamClient(router, *this));
    client->accept<Proto>(acceptor);
}
