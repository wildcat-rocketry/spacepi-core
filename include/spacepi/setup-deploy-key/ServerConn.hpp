#ifndef SPACEPI_TARGET_SERVERCONN_HPP
#define SPACEPI_TARGET_SERVERCONN_HPP

#include <boost/asio.hpp>
#include <memory>
#include <spacepi/log/AutoLog.hpp>

namespace spacepi {
    namespace target {
        namespace deployKey {
            class ServerConn : public std::enable_shared_from_this<ServerConn> , private spacepi::log::AutoLog<decltype("setup-deploy-key"_autolog)>{

                public:
                    ServerConn();
                    ~ServerConn();
                    ServerConn(const ServerConn &) = delete;
                    ServerConn & operator = (const ServerConn &) = delete;
                    boost::asio::ip::tcp::socket& getSocket();
                    void connReady();

                private:
                    boost::asio::ip::tcp::socket socket;

            };
        }
    }
}

#endif
