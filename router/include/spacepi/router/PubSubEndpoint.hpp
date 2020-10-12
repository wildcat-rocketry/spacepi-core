#ifndef SPACEPI_CORE_ROUTER_PUBSUBENDPOINT_HPP
#define SPACEPI_CORE_ROUTER_PUBSUBENDPOINT_HPP

#include <cstdint>
#include <string>

namespace spacepi {
    namespace router {
        class PubSubRouter;

        class PubSubEndpoint {
            friend class PubSubRouter;

            public:
                PubSubEndpoint(const PubSubEndpoint &) = delete;
                virtual ~PubSubEndpoint();

                PubSubEndpoint &operator =(const PubSubEndpoint &) = delete;

            protected:
                PubSubEndpoint(PubSubRouter *router);

                void publish(uint32_t id, const std::string &data);
                virtual void handlePublish(uint32_t id, const std::string &data) = 0;

            private:
                PubSubRouter *router;
        };
    }
}

#endif
