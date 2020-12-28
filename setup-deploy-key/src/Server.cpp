#include <boost/asio.hpp>
#include <spacepi/setup-deploy-key/Server.hpp>
#include <spacepi/setup-deploy-key/ServerConn.hpp>
#include <SpacePi.hpp>
#include <cstdint>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_link.h>
#include <mutex>

using namespace boost::asio::ip;
using namespace boost::system;
using namespace spacepi::target::deployKey;
using namespace spacepi::log;
using namespace spacepi::messaging::network;

void ServerAcceptCallback::operator()(const error_code& error){
    if(error) {
        serverPtr->log(LogLevel::Error) << "Error accepting client: " << error;
    }
    else {
        serverPtr->serverConnPtr->connReady(); 
    }
    serverPtr->acceptClient();
}
ServerAcceptCallback::ServerAcceptCallback(Server* serverPtr) : serverPtr(serverPtr){
}

Server::Server(uint16_t portNum) : tcpAcceptor(NetworkThread::instance.getContext(),tcp::endpoint(address::from_string("0.0.0.0"), portNum)) {
    log(LogLevel::Info) << "Ready to connect to Github";
    log(LogLevel::Info) << "Please navigate to one of the following addresses in your browser to complete setup:";
    struct ifaddrs *ifaddr;
    int family, s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (struct ifaddrs *ifa = ifaddr; ifa != NULL;
            ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        if (family == AF_INET) {
            s = getnameinfo(ifa->ifa_addr,
                    (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                            sizeof(struct sockaddr_in6),
                    host, NI_MAXHOST,
                    NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
            if(std::string(host).substr(0,3) != "127") {
                log(LogLevel::Info) << "http://" << host << ":8000";
            }

        } else if (family == AF_PACKET && ifa->ifa_data != NULL) {
            struct rtnl_link_stats *stats = (rtnl_link_stats*)ifa->ifa_data;
        }
    }

    freeifaddrs(ifaddr);
    acceptClient();
}

void Server::setCallbackCode(std::string str){
    std::unique_lock<std::mutex> mutexlock(mutualexclusion);
    Server::callbackcode = str;
    waiting.notify_all();
}

std::string Server::getCallbackCode() {
    return callbackcode;
}

void Server::waitForCallbackCode() {
    std::unique_lock<std::mutex> mutexlock(mutualexclusion);
    while(callbackcode.empty()){
        waiting.wait(mutexlock);
    }
}

void Server::acceptClient(){
    serverConnPtr.reset(new ServerConn(this));
    tcpAcceptor.async_accept(serverConnPtr->getSocket(),ServerAcceptCallback(this));
}
