#ifndef SPACEPI_TARGET_SERVERCONN_HPP
#define SPACEPI_TARGET_SERVERCONN_HPP

#include <boost/asio.hpp>
#include <memory>
#include <spacepi/log/AutoLog.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>

namespace spacepi {
    namespace target {
        namespace deployKey {
            class ServerConn;
            class ServerConnReadCallback {
                friend class ServerConn;
                public:
                    void operator()(const boost::system::error_code& error,size_t transbyte);

                private:
                    ServerConnReadCallback(std::shared_ptr<ServerConn> serverConnPtr);
                    std::shared_ptr<ServerConn> serverConnPtr;
            };
            class ServerConn : public std::enable_shared_from_this<ServerConn> , private spacepi::log::AutoLog<decltype("setup-deploy-key"_autolog)>{
                friend class ServerConnReadCallback;
                public:
                    ServerConn();
                    ~ServerConn();
                    ServerConn(const ServerConn &) = delete;
                    ServerConn & operator = (const ServerConn &) = delete;
                    boost::asio::ip::tcp::socket& getSocket();
                    void connReady();

                private:
                    boost::asio::ip::tcp::socket socket;
                    boost::beast::flat_buffer buffer;
                    boost::beast::http::request<boost::beast::http::empty_body> httprequest;

            };
        }
    }
}

#endif
