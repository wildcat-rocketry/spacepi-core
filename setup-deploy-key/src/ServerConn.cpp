#include <boost/asio.hpp>
#include <spacepi/setup-deploy-key/ServerConn.hpp>
#include <spacepi/messaging/network/NetworkThread.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>

using namespace boost::asio::ip;
using namespace spacepi::target::deployKey;
using namespace spacepi::messaging::network;
using namespace spacepi::log;
using namespace boost::beast::http;
using namespace boost::system;

void ServerConnReadCallback::operator()(const error_code& error,size_t transbyte){
    if(error){
        serverConnPtr->log(LogLevel::Error) << "Error reading: " << error;
    }
    else {
        serverConnPtr->connReady(); 
    }
    serverConnPtr->connReady();
}

ServerConnReadCallback::ServerConnReadCallback(ServerConn* serverConnPtr) : serverConnPtr(serverConnPtr){

}
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
    async_read(socket,buffer,httprequest,ServerConnReadCallback(this));
}
