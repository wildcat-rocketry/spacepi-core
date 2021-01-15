#ifndef SPACEPI_CORE_ROUTER_ROUTERENDPOINT_HPP
#define SPACEPI_CORE_ROUTER_ROUTERENDPOINT_HPP

#include <ostream>
#include <string>

namespace spacepi {
    namespace router {
        class RouterEndpoint {
            public:
                enum Type {
                    Invalid,
                    TCP,
                    UNIX
                };

                static RouterEndpoint defaultTCPEndpoint;
                static RouterEndpoint defaultUNIXEndpoint;

                RouterEndpoint() noexcept;
                RouterEndpoint(int port) noexcept;
                RouterEndpoint(const std::string &path) noexcept;

                static bool tryParse(const std::string &str, RouterEndpoint &endpoint) noexcept;
                std::string toString() const noexcept;

                enum Type getType() const noexcept;
                int getPort() const noexcept;
                const std::string &getPath() const noexcept;

            private:
                enum Type type;
                int port;
                std::string path;
        };
    }
}

std::ostream &operator <<(std::ostream &os, const spacepi::router::RouterEndpoint &endpoint);

#endif
