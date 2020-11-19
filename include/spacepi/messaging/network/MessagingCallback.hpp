#ifndef SPACEPI_CORE_MESSAGING_NETWORK_MESSAGINGCALLBACK_HPP
#define SPACEPI_CORE_MESSAGING_NETWORK_MESSAGINGCALLBACK_HPP

#include <string>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/util/Exception.hpp>

namespace spacepi {
    namespace messaging {
        namespace network {
            namespace detail {
                class MessagingSocketAcceptor;
                class MessagingSocketConnector;
            }

            class MessagingSocket;

            class MessagingCallback {
                friend class MessagingSocket;
                friend class detail::MessagingSocketAcceptor;
                friend class detail::MessagingSocketConnector;

                protected:
                    virtual void handleMessage(const SubscriptionID &id, const std::string &msg) = 0;
                    virtual void handleAccept();
                    virtual void handleConnect();
                    virtual void handleError(const spacepi::util::Exception::pointer &err) = 0;
            };
        }
    }
}

#endif
