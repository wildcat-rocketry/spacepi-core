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

            /**
             * \brief Callback class for events from the MessagingSocket
             */
            class MessagingCallback {
                friend class MessagingSocket;
                friend class detail::MessagingSocketAcceptor;
                friend class detail::MessagingSocketConnector;

                protected:
                    /**
                     * \brief Called after a message from the other end of the socket is received
                     * 
                     * \param[in] id The SubscriptionID describing the message data type
                     * \param[in] msg The serialized binary representation of the message
                     */
                    virtual void handleMessage(const SubscriptionID &id, const std::string &msg) = 0;

                    /**
                     * \brief Called after a remote client has connected to the server socket successfully
                     */
                    virtual void handleAccept();

                    /**
                     * \brief Called after a client socket has connected to a remove server successfully
                     */
                    virtual void handleConnect();

                    /**
                     * \brief Called any time there is an error during an IO operation with the socket
                     * 
                     * \param[in] err The error which occurred
                     */
                    virtual void handleError(const spacepi::util::Exception::pointer &err) = 0;
            };
        }
    }
}

#endif
