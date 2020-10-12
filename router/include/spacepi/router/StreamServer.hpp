#ifndef SPACEPI_CORE_ROUTER_STREAMSERVER_HPP
#define SPACEPI_CORE_ROUTER_STREAMSERVER_HPP

#include <memory>
#include <vector>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/router/StreamClient.hpp>
#include <spacepi/router/PubSubRouter.hpp>

namespace spacepi {
    namespace router {
        template <typename Proto>
        class StreamServer : public StreamClientCallback, private spacepi::log::AutoLog<decltype("core:router"_autolog)> {
            public:
                template <typename Endpoint>
                StreamServer(PubSubRouter *router, const Endpoint &endpoint);
                StreamServer(const StreamServer<Proto> &) = delete;

                StreamServer<Proto> &operator =(const StreamServer<Proto> &) = delete;

            protected:
                void handleAccept();
                void handleAcceptError(spacepi::util::Exception::pointer err);
                void handleError(spacepi::util::Exception::pointer err);

            private:
                void startAccept();

                boost::asio::basic_socket_acceptor<Proto> acceptor;
                std::vector<std::shared_ptr<StreamClient>> clients;
                std::shared_ptr<StreamClient> newClient;
                PubSubRouter *router;
        };
    }
}

#endif
