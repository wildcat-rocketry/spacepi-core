#ifndef SPACEPI_CORE_ROUTER_ROUTERSERVER_HPP
#define SPACEPI_CORE_ROUTER_ROUTERSERVER_HPP

#include <memory>
#include <vector>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <spacepi/router/Client.hpp>

namespace spacepi {
    namespace router {
        template <typename Proto>
        class RouterServer {
            public:
                template <typename Endpoint>
                RouterServer(boost::asio::io_context &ctx, Endpoint endpoint);
                RouterServer(const RouterServer<Proto> &) = delete;
                ~RouterServer();

                RouterServer &operator =(const RouterServer &) = delete;

            private:
                void doAccept();
                void handleAccept(std::shared_ptr<Client<Proto>> client, const boost::system::error_code &err);

                boost::asio::io_context &ctx;
                boost::asio::basic_socket_acceptor<Proto> acceptor;
                std::vector<std::shared_ptr<Client<Proto>>> clients;
        };
    }
}

#endif
