#include <string>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/router/PubSubEndpoint.hpp>
#include <spacepi/router/PubSubRouter.hpp>

using namespace spacepi::messaging::network;
using namespace spacepi::router;

PubSubEndpoint::~PubSubEndpoint() {
    router->unregister(this);
}

PubSubEndpoint::PubSubEndpoint(PubSubRouter *router) : router(router) {
}

void PubSubEndpoint::publish(const SubscriptionID &id, const std::string &data) {
    router->publish(this, id, data);
}
