#ifndef SPACEPI_CORE_ROUTER_ROUTER_HPP
#define SPACEPI_CORE_ROUTER_ROUTER_HPP

#include <memory>
#include <vector>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/router/RouterEndpoint.hpp>
#include <spacepi/router/PubSubRouter.hpp>
#include <spacepi/router/StreamClient.hpp>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

namespace spacepi {
    namespace router {
        class Router : private spacepi::log::AutoLog<decltype("router"_autolog)>, private spacepi::util::CommandConfigurable {
            public:
                Router(spacepi::util::Command &cmd) noexcept;

                void runCommand();

            private:
                std::vector<RouterEndpoint> routerEndpoints;
                PubSubRouter pubsub;
                std::vector<std::shared_ptr<StreamClientCallback>> servers;
        };
    }
}

#endif
