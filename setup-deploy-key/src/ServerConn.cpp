#include <boost/asio.hpp>
#include <spacepi/setup-deploy-key/ServerConn.hpp>
#include <spacepi/messaging/network/NetworkThread.hpp>
#include <spacepi/log/LogLevel.hpp>

using namespace boost::asio::ip;
using namespace spacepi::target::deployKey;
using namespace spacepi::messaging::network;
using namespace spacepi::log;

ServerConn::ServerConn() : socket(NetworkThread::instance.getContext()) {
    
}

ServerConn::~ServerConn() {
    log(LogLevel::Info) << "Client connection lost from server.";
}

tcp::socket& ServerConn::getSocket(){
    return socket;
}

void ServerConn::connReady() {
    log(LogLevel::Info) << "Client connected to server.";
    sleep(1);
}
