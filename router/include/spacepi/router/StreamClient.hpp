#ifndef SPACEPI_CORE_ROUTER_STREAMCLIENT_HPP
#define SPACEPI_CORE_ROUTER_STREAMCLIENT_HPP

#include <string>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/messaging/network/MessagingSocket.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/router/PubSubEndpoint.hpp>
#include <spacepi/router/PubSubRouter.hpp>
#include <spacepi/router/StreamClientCallback.hpp>
#include <spacepi/util/Exception.hpp>

namespace spacepi {
    namespace router {
        class StreamClient : public PubSubEndpoint, public spacepi::messaging::network::MessagingSocket, public spacepi::messaging::network::MessagingCallback, private spacepi::log::AutoLog<decltype("core:router"_autolog)> {
            public:
                StreamClient(PubSubRouter &router, StreamClientCallback &callback) noexcept;

                StreamClient(const StreamClient &) = delete;
                StreamClient &operator =(const StreamClient &) = delete;

                void sendHello();

            protected:
                void handlePublish(const spacepi::messaging::network::SubscriptionID &id, const std::string &data);
                void handleMessage(const spacepi::messaging::network::SubscriptionID &id, const std::string &msg);
                void handleAccept();
                void handleError(const spacepi::util::Exception::pointer &err);

            private:
                StreamClientCallback &callback;
                bool accepted;
        };
    }
}

#endif
