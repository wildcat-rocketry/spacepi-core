#include <boost/asio.hpp>
#include <spacepi/memory/SocketDeleter.hpp>
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::asio::local;
using namespace spacepi::memory;

SocketDeleter::SocketDeleter() {
}

template <>
void SocketDeleter::operator ()(basic_socket<tcp> *ptr) const {
    delete (tcp::socket *) ptr;
}

template <>
void SocketDeleter::operator ()(basic_socket<udp> *ptr) const {
    delete (udp::socket *) ptr;
}

template <>
void SocketDeleter::operator ()(basic_socket<stream_protocol> *ptr) const {
    delete (stream_protocol::socket *) ptr;
}

template <>
void SocketDeleter::operator ()(basic_stream_socket<tcp> *ptr) const {
    delete (tcp::socket *) ptr;
}

template <>
void SocketDeleter::operator ()(basic_stream_socket<stream_protocol> *ptr) const {
    delete (stream_protocol::socket *) ptr;
}
