#include <boost/asio.hpp>
#include <spacepi/messaging/network/NetworkThread.hpp>
#include <spacepi/setup-deploy-key/Server.hpp>
#include <spacepi/setup-deploy-key/ServerConn.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <cstdint>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_link.h>

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
    log(LogLevel::Info) << "Server started on port " << portNum; 
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
    exit(EXIT_SUCCESS);
    acceptClient();
}

void Server::acceptClient(){
    serverConnPtr.reset(new ServerConn());
    tcpAcceptor.async_accept(serverConnPtr->getSocket(),ServerAcceptCallback(this));
}

