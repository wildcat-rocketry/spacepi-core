#ifndef SPACEPI_CORE_ROUTER_STREAMSERVER_HPP
#define SPACEPI_CORE_ROUTER_STREAMSERVER_HPP

#include <memory>
#include <vector>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/router/StreamClient.hpp>
#include <spacepi/router/PubSubRouter.hpp>

namespace spacepi {
    namespace router {
        template <typename Proto>
        class StreamServer : public StreamClientCallback, private spacepi::log::AutoLog<decltype("core:router"_autolog)> {
            public:
                StreamServer(PubSubRouter &router, const typename Proto::endpoint &endpoint);

                StreamServer(const StreamServer<Proto> &) = delete;
                StreamServer<Proto> &operator =(const StreamServer<Proto> &) = delete;

            protected:
                void handleAccept();
                void handleAcceptError(const spacepi::util::Exception::pointer &err);
                void handleError(const spacepi::util::Exception::pointer &err);

            private:
                void startAccept();

                typename Proto::acceptor acceptor;
                std::shared_ptr<StreamClient> client;
                PubSubRouter &router;
        };
    }
}

#endif
