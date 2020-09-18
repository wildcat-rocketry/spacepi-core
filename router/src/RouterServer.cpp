#include <memory>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/system/error_code.hpp>
#include <spacepi/router/Client.hpp>
#include <spacepi/router/RouterServer.hpp>
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::asio::local;
using namespace boost::system;
using namespace spacepi::router;

template class RouterServer<tcp>;
template RouterServer<tcp>::RouterServer(io_context &, tcp::endpoint);
template class RouterServer<udp>;
template RouterServer<udp>::RouterServer(io_context &, udp::endpoint);
template class RouterServer<stream_protocol>;
template RouterServer<stream_protocol>::RouterServer(io_context &, stream_protocol::endpoint);

template <typename Proto>
template <typename Endpoint>
RouterServer<Proto>::RouterServer(io_context &ctx, Endpoint endpoint) : ctx(ctx), acceptor(ctx, endpoint) {
    doAccept();
}

template <typename Proto>
RouterServer<Proto>::~RouterServer() {
}

template <typename Proto>
void RouterServer<Proto>::doAccept() {
    std::shared_ptr<Client<Proto>> client(new Client<Proto>(ctx, this));
    acceptor.async_accept(client->getSocket(), bind(&RouterServer<Proto>::handleAccept, this, client, asio::placeholders::error));
}

template <typename Proto>
void RouterServer<Proto>::handleAccept(std::shared_ptr<Client<Proto>> client, const error_code &err) {
    if (!err) {
        clients.push_back(client);
        client->start();
    }
    doAccept();
}
