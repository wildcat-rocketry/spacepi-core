#ifndef SPACEPI_TARGETLIB_LINUX_SETUP_DEPLOY_KEY_SERVER_HPP
#define SPACEPI_TARGETLIB_LINUX_SETUP_DEPLOY_KEY_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <condition_variable>
#include <cstdint>
#include <memory>
#include <mutex>
#include <spacepi/setup-deploy-key/ServerConn.hpp>
#include <spacepi/log/AutoLog.hpp>
#include <SpacePi.hpp>

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
                    void setCallbackCode(std::string str);
                    std::string getCallbackCode();
                    void waitForCallbackCode();
                    

                private:
                    boost::asio::ip::tcp::acceptor tcpAcceptor;
                    void acceptClient();
                    std::shared_ptr<ServerConn> serverConnPtr;
                    std::string callbackcode;
                    std::mutex mutualexclusion;
                    std::condition_variable waiting;
            };
        }
    }
}

#endif
