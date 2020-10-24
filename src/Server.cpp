#include <boost/asio.hpp>
#include <spacepi/messaging/network/NetworkThread.hpp>
#include <spacepi/setup-deploy-key/Server.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <cstdint>

using namespace boost::asio::ip;
using namespace boost::system;
using namespace spacepi::target::deployKey;
using namespace spacepi::log;
using namespace spacepi::messaging::network;

void ServerAcceptCallback::operator()(const error_code& error){
    if(error) {
        serverPtr->log(LogLevel::Error) << "Error accepting client: " << error;
    }
    serverPtr->acceptClient();
}
ServerAcceptCallback::ServerAcceptCallback(Server* serverPtr) : serverPtr(serverPtr){

}

Server::Server(uint16_t portNum) : tcpAcceptor(NetworkThread::instance.getContext(),tcp::endpoint(address::from_string("0.0.0.0"), portNum)) {
    log(LogLevel::Info) << "Server started on port " << portNum;
    acceptClient();
}

void Server::acceptClient(){
    tcp::socket socket(NetworkThread::instance.getContext());
    tcpAcceptor.async_accept(socket,ServerAcceptCallback(this));
}
