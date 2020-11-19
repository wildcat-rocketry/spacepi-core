#ifndef SPACEPI_CORE_ROUTER_PUBSUBENDPOINT_HPP
#define SPACEPI_CORE_ROUTER_PUBSUBENDPOINT_HPP

#include <string>
#include <spacepi/messaging/network/SubscriptionID.hpp>

namespace spacepi {
    namespace router {
        class PubSubRouter;

        class PubSubEndpoint {
            friend class PubSubRouter;

            public:
                ~PubSubEndpoint() noexcept;

                PubSubEndpoint(PubSubEndpoint &) = delete;
                PubSubEndpoint &operator =(PubSubEndpoint &) = delete;

            protected:
                explicit PubSubEndpoint(PubSubRouter &router) noexcept;

                void publish(const spacepi::messaging::network::SubscriptionID &id, const std::string &data);
                virtual void handlePublish(const spacepi::messaging::network::SubscriptionID &id, const std::string &data) = 0;

            private:
                PubSubRouter &router;
        };
    }
}

#endif
