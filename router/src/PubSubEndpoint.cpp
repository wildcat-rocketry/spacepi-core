#include <cstdint>
#include <string>
#include <spacepi/router/PubSubEndpoint.hpp>
#include <spacepi/router/PubSubRouter.hpp>

using namespace spacepi::router;

PubSubEndpoint::~PubSubEndpoint() {
    router->unregister(this);
}

PubSubEndpoint::PubSubEndpoint(PubSubRouter *router) : router(router) {
}

void PubSubEndpoint::publish(uint32_t id, const std::string &data) {
    router->publish(this, id, data);
}
