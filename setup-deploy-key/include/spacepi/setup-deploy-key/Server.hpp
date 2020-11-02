#ifndef SPACEPI_TARGET_SERVER_HPP
#define SPACEPI_TARGET_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <cstdint>
#include <spacepi/log/AutoLog.hpp>
#include <memory>
#include <spacepi/setup-deploy-key/ServerConn.hpp>

namespace spacepi{
    namespace target {
        namespace deployKey {
            class Server;

            class ServerAcceptCallback {
                friend class Server;
                public:
                    void operator()(const boost::system::error_code& error);

                private:
                    ServerAcceptCallback(Server* serverPtr);
                    Server* serverPtr;
            };

            class Server : private spacepi::log::AutoLog<decltype("setup-deploy-key"_autolog)> { 
                friend class ServerAcceptCallback;
                public:
                    Server(uint16_t portNum);
                    Server(const Server &) = delete; 
                    Server & operator = (const Server &) = delete; 
                    

                private:
                    boost::asio::ip::tcp::acceptor tcpAcceptor;
                    void acceptClient();
                    std::shared_ptr<ServerConn> serverConnPtr;
            };
        }
    }
}

#endif
