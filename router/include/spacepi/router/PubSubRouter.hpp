#ifndef SPACEPI_CORE_ROUTER_PUBSUBROUTER_HPP
#define SPACEPI_CORE_ROUTER_PUBSUBROUTER_HPP

#include <condition_variable>
#include <string>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <spacepi/concurrent/RWMutex.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/router/PubSubEndpoint.hpp>

namespace spacepi {
    namespace router {
        class PubSubRouter {
            public:
                PubSubRouter();
                PubSubRouter(const PubSubRouter &) = delete;

                PubSubRouter &operator =(const PubSubRouter &) = delete;

                void unregister(PubSubEndpoint *endpoint);
                void publish(PubSubEndpoint *sender, const spacepi::messaging::network::SubscriptionID &id, const std::string &data);

            private:
                std::unordered_set<PubSubEndpoint *> fullSubscriptions;
                std::unordered_map<spacepi::messaging::network::SubscriptionID, std::unordered_set<PubSubEndpoint *>> subscriptions;
                spacepi::concurrent::RWMutex<std::mutex, std::unique_lock<std::mutex>, std::condition_variable> mtx;
        };
    }
}

#endif
