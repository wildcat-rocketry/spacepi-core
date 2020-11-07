#include <boost/asio.hpp>
#include <spacepi/setup-deploy-key/ServerConn.hpp>
#include <spacepi/messaging/network/NetworkThread.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>
#include <memory>

using namespace boost;
using namespace boost::asio::ip;
using namespace spacepi::target::deployKey;
using namespace spacepi::messaging::network;
using namespace spacepi::log;
using namespace boost::beast::http;
using namespace boost::system;
using namespace std;

void ServerConnReadCallback::operator()(const system::error_code& error,size_t transbyte){
    if(error){
        serverConnPtr->log(LogLevel::Error) << "Error reading: " << error;
    }
    else {
        //serverConnPtr->log(LogLevel::Info) << serverConnPtr->httprequest.target(); //USE THIS L8R
        serverConnPtr->response.result(status::temporary_redirect);
        serverConnPtr->response.set("Location","https://ffaero.com/static/github-auth?target=" + serverConnPtr->httprequest["Host"].to_string()+"%2Fcallback&scope=write%3Apublic_key&allow_signup=false");
        async_write(serverConnPtr->socket,serverConnPtr->response,ServerConnWriteCallback(serverConnPtr));
    }
}

ServerConnReadCallback::ServerConnReadCallback(std::shared_ptr<ServerConn> serverConnPtr) : serverConnPtr(serverConnPtr){

}

void ServerConnWriteCallback::operator()(const system::error_code& error, size_t transbyte){
    if(error){
        serverConnPtr->log(LogLevel::Error) << "Error writing to server. " << error;
    }
    else {
        
    } 
}

ServerConnWriteCallback::ServerConnWriteCallback(std::shared_ptr<ServerConn> serverConnPtr) : serverConnPtr(serverConnPtr) {

}

ServerConn::ServerConn() : socket(NetworkThread::instance.getContext()) {
    
}


tcp::socket& ServerConn::getSocket(){
    return socket;
}

void ServerConn::connReady() {
    async_read(socket,buffer,httprequest,ServerConnReadCallback(shared_from_this()));

}
