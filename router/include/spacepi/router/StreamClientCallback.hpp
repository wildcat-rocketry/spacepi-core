#ifndef SPACEPI_CORE_ROUTER_STREAMCLIENTCALLBACK_HPP
#define SPACEPI_CORE_ROUTER_STREAMCLIENTCALLBACK_HPP

#include <spacepi/util/Exception.hpp>

namespace spacepi {
    namespace router {
        class StreamClient;

        class StreamClientCallback {
            friend class StreamClient;

            protected:
                virtual void handleAccept() = 0;
                virtual void handleAcceptError(const spacepi::util::Exception::pointer &err) = 0;
                virtual void handleError(const spacepi::util::Exception::pointer &err) = 0;
        };
    }
}

#endif
