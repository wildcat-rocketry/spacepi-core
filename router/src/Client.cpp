#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/system/error_code.hpp>
#include <spacepi/router/Client.hpp>
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::asio::local;
using namespace boost::system;
using namespace spacepi::router;

template class Client<tcp>;
template class Client<udp>;
template class Client<stream_protocol>;

template <>
Client<tcp>::Client(io_context &ctx, RouterServer<tcp> *server) : socket(new tcp::socket(ctx)), server(server) {
}

template <>
Client<udp>::Client(io_context &ctx, RouterServer<udp> *server) : socket(new udp::socket(ctx)), server(server) {
}

template <>
Client<stream_protocol>::Client(io_context &ctx, RouterServer<stream_protocol> *server) : socket(new stream_protocol::socket(ctx)), server(server) {
}

template <typename Proto>
Client<Proto>::~Client() {
}

template <typename Proto>
void Client<Proto>::start() {
    write(buffer("SpacePi Router v2.0\n"));
}

template <>
void Client<tcp>::write(const const_buffer &buf) {
    async_write(*(tcp::socket *) socket.get(), buf, bind(&Client<tcp>::handleWrite, this, asio::placeholders::error));
}

template <>
void Client<udp>::write(const const_buffer &buf) {
    ((udp::socket *) socket.get())->async_send(buf, bind(&Client<udp>::handleWrite, this, asio::placeholders::error));
}

template <>
void Client<stream_protocol>::write(const const_buffer &buf) {
    async_write(*(stream_protocol::socket *) socket.get(), buf, bind(&Client<stream_protocol>::handleWrite, this, asio::placeholders::error));
}

template <typename Proto>
boost::asio::basic_socket<Proto> &Client<Proto>::getSocket() {
    return *socket;
}

template <typename Proto>
void Client<Proto>::handleWrite(const error_code &err) {
}
