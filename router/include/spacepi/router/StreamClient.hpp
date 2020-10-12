#ifndef SPACEPI_CORE_ROUTER_STREAMCLIENT_HPP
#define SPACEPI_CORE_ROUTER_STREAMCLIENT_HPP

#include <cstdint>
#include <string>
#include <spacepi/messaging/network/MessagingSocket.hpp>
#include <spacepi/router/PubSubEndpoint.hpp>
#include <spacepi/router/PubSubRouter.hpp>
#include <spacepi/util/Exception.hpp>

namespace spacepi {
    namespace router {
        class StreamClient;

        class StreamClientCallback {
            friend class StreamClient;

            protected:
                virtual void handleAccept() = 0;
                virtual void handleAcceptError(spacepi::util::Exception::pointer err) = 0;
                virtual void handleError(spacepi::util::Exception::pointer err) = 0;
        };

        class StreamClient : public PubSubEndpoint, public spacepi::messaging::network::MessagingSocket, public spacepi::messaging::network::MessagingCallback {
            public:
                StreamClient(PubSubRouter *router, StreamClientCallback *callback);
                StreamClient(const StreamClient &) = delete;

                StreamClient &operator =(const StreamClient &) = delete;

                void sendHello();

            protected:
                void handlePublish(uint32_t id, const std::string &data);
                void handleMessage(uint32_t id, const std::string &msg);
                void handleAccept();
                void handleError(spacepi::util::Exception::pointer err);

            private:
                StreamClientCallback *callback;
                bool accepted;
        };
    }
}

#endif
