#ifndef SPACEPI_CORE_ROUTER_UNIXSTREAMSERVERWRAPPER_HPP
#define SPACEPI_CORE_ROUTER_UNIXSTREAMSERVERWRAPPER_HPP

#include <memory>
#include <string>
#include <boost/asio.hpp>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/messaging/network/MessagingSocket.hpp>
#include <spacepi/messaging/network/MessagingCallback.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/router/PubSubRouter.hpp>
#include <spacepi/router/RouterEndpoint.hpp>
#include <spacepi/router/StreamClientCallback.hpp>
#include <spacepi/router/StreamServer.hpp>
#include <spacepi/util/Exception.hpp>

namespace spacepi {
    namespace router {
        class UnixStreamServerWrapper : public StreamClientCallback, public spacepi::messaging::network::MessagingCallback, private spacepi::log::AutoLog<decltype("core:router"_autolog)> {
            public:
                UnixStreamServerWrapper(PubSubRouter &router, const RouterEndpoint &endpoint);

                UnixStreamServerWrapper(UnixStreamServerWrapper &) = delete;
                UnixStreamServerWrapper &operator =(UnixStreamServerWrapper &) = delete;

            protected:
                void handleAccept(const spacepi::util::Exception::pointer *err);
                void handleMessage(const spacepi::messaging::network::SubscriptionID &id, const std::string &msg);
                void handleAccept();
                void handleConnect();
                void handleError(const spacepi::util::Exception::pointer &err);

            private:
                void start();

                PubSubRouter &router;
                const RouterEndpoint &endpoint;
                std::unique_ptr<spacepi::messaging::network::MessagingSocket> tester;
                std::unique_ptr<StreamServer<boost::asio::local::stream_protocol>> server;
        };
    }
}

#endif
