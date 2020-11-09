#include <boost/asio.hpp>
#include <spacepi/setup-deploy-key/ServerConn.hpp>
#include <spacepi/messaging/network/NetworkThread.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>
#include <memory>
#include <spacepi/setup-deploy-key/Server.hpp>
#include <spacepi/messaging/network/NetworkThread.hpp>

using namespace boost;
using namespace boost::asio::ip;
using namespace spacepi::target::deployKey;
using namespace spacepi::messaging::network;
using namespace spacepi::log;
using namespace boost::beast::http;
using namespace boost::system;
using namespace std;

void ServerConnReadCallback::operator()(const system::error_code& error,size_t transbyte){
    std::string url = serverConnPtr->httprequest.target().to_string();
    if(error){
        serverConnPtr->log(LogLevel::Error) << "Error reading: " << error;
    }
    else {
        //serverConnPtr->log(LogLevel::Info) << serverConnPtr->httprequest.target(); //USE THIS L8R
        if(url.substr(0,14) == ("/callback?code")) {
            serverConnPtr->log(LogLevel::Info) << "Found Callback String";
            std::string tempcode = url.substr(15);
            serverConnPtr->serverPtr->setCallbackCode(tempcode);
        }
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
}

ServerConnWriteCallback::ServerConnWriteCallback(std::shared_ptr<ServerConn> serverConnPtr) : serverConnPtr(serverConnPtr) {

}

ServerConn::ServerConn(Server* serverPtr) : socket(NetworkThread::instance.getContext()), serverPtr(serverPtr) {
    
}


tcp::socket& ServerConn::getSocket(){
    return socket;
}

void ServerConn::connReady() {
    async_read(socket,buffer,httprequest,ServerConnReadCallback(shared_from_this()));

}




