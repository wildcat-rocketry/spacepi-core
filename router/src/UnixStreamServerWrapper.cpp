#include <string>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/messaging/network/MessagingSocket.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/router/PubSubRouter.hpp>
#include <spacepi/router/RouterEndpoint.hpp>
#include <spacepi/router/StreamServer.hpp>
#include <spacepi/router/UnixStreamServerWrapper.hpp>
#include <spacepi/util/Exception.hpp>

using namespace std;
using namespace boost::asio::local;
using namespace boost::filesystem;
using namespace spacepi::log;
using namespace spacepi::messaging::network;
using namespace spacepi::router;
using namespace spacepi::util;

UnixStreamServerWrapper::UnixStreamServerWrapper(PubSubRouter &router, const RouterEndpoint &endpoint) : router(router), endpoint(endpoint) {
    if (endpoint.getType() != RouterEndpoint::UNIX) {
        throw EXCEPTION(ArgumentException("UnixStreamServerWrapper only works on UNIX sockets."));
    }
    const string &path = endpoint.getPath();
    if (exists(path)) {
        tester.reset(new MessagingSocket(*this));
        tester->connect<stream_protocol>(stream_protocol::endpoint(path));
    } else {
        start();
    }
}

void UnixStreamServerWrapper::handleAccept(const Exception::pointer *err) {
    throw EXCEPTION(StateException("Method should never be called."));
}

void UnixStreamServerWrapper::handleMessage(const SubscriptionID &id, const string &msg) {
}

void UnixStreamServerWrapper::handleAccept() {
    throw EXCEPTION(StateException("Method should never be called."));
}

void UnixStreamServerWrapper::handleConnect() {
    tester.reset();
    log(LogLevel::Error) << "Another server is already running on " << endpoint;
}

void UnixStreamServerWrapper::handleError(const Exception::pointer &err) {
    tester.reset();
    log(LogLevel::Debug) << "Deleting old socket file.";
    remove(endpoint.getPath());
    start();
}

void UnixStreamServerWrapper::start() {
    server.reset(new StreamServer<stream_protocol>(router, endpoint, stream_protocol::endpoint(endpoint.getPath())));
}
