#ifndef SPACEPI_CORE_ROUTER_CLIENT_HPP
#define SPACEPI_CORE_ROUTER_CLIENT_HPP

#include <memory>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <spacepi/memory/SocketDeleter.hpp>

namespace spacepi {
    namespace router {
        template <typename Proto>
        class RouterServer;

        template <typename Proto>
        class Client {
            public:
                Client(boost::asio::io_context &ctx, RouterServer<Proto> *server);
                Client(const Client &) = delete;
                ~Client();

                Client &operator =(const Client &) = delete;

                void start();
                void write(const boost::asio::const_buffer &buf);
                boost::asio::basic_socket<Proto> &getSocket();

            private:
                void handleWrite(const boost::system::error_code &err);

                std::unique_ptr<boost::asio::basic_socket<Proto>, spacepi::memory::SocketDeleter> socket;
                RouterServer<Proto> *server;
        };
    }
}

#endif
