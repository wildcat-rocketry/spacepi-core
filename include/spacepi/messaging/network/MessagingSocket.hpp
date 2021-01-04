#ifndef SPACEPI_CORE_MESSAGING_NETWORK_MESSAGINGSOCKET_HPP
#define SPACEPI_CORE_MESSAGING_NETWORK_MESSAGINGSOCKET_HPP

#include <memory>
#include <string>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <spacepi/messaging/network/MessagingCallback.hpp>
#include <spacepi/messaging/network/SocketWrapper.hpp>
#include <spacepi/messaging/network/SocketWrapperCallback.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/util/Exception.hpp>
#include <spacepi/util/SharedOrRef.hpp>

namespace spacepi {
    namespace messaging {
        namespace network {
            class MessagingSocket;

            namespace detail {
                class MessagingSocketAcceptor {
                    friend class network::MessagingSocket;

                    public:
                        void operator ()(const boost::system::error_code &err);

                    private:
                        explicit MessagingSocketAcceptor(MessagingSocket &socket) noexcept;

                        spacepi::util::SharedOrRef<MessagingSocket> socket;
                };

                class MessagingSocketConnector {
                    friend class network::MessagingSocket;

                    public:
                        void operator ()(const boost::system::error_code &err);

                    private:
                        explicit MessagingSocketConnector(MessagingSocket &socket) noexcept;

                        spacepi::util::SharedOrRef<MessagingSocket> socket;
                };
            }

            /**
             * \brief High-level socket which can send and receive serialized messages with SubscriptionID's
             */
            class MessagingSocket : public SocketWrapperCallback, public std::enable_shared_from_this<MessagingSocket> {
                friend class detail::MessagingSocketAcceptor;
                friend class detail::MessagingSocketConnector;

                public:
                    /**
                     * \brief Initializes a MessagingSocket which is not connected to anything
                     * 
                     * \param[in,out] callback The callback to call when events occur
                     */
                    explicit MessagingSocket(MessagingCallback &callback) noexcept;

                    MessagingSocket(MessagingSocket &) = delete;
                    MessagingSocket &operator =(MessagingSocket &) = delete;

                    /**
                     * \brief Start asynchronously attempting to allow a remote client to connect to the server socket
                     * 
                     * This method is defined for the following protocols:
                     * \li \c boost::asio::ip::tcp
                     * \li \c boost::asio::local::stream_protocol
                     * 
                     * \param[in] acceptor The server socket to accept on
                     */
                    template <typename Proto>
                    void accept(typename Proto::acceptor &acceptor);

                    /**
                     * \brief Start asynchronously attempting to connect to a remote server
                     * 
                     * This method is defined for the following protocols:
                     * \li \c boost::asio::ip::tcp
                     * \li \c boost::asio::local::stream_protocol
                     * 
                     * \param[in] endpoint The address of the remote server
                     */
                    template <typename Proto>
                    void connect(const typename Proto::endpoint &endpoint);

                    /**
                     * \brief Sends a message to the MessagingSocket on the other side of the connection
                     * 
                     * When this message is received, it will get passed to MessagingCallback::handleMessage
                     * 
                     * \param[in] id The SubscriptionID describing the message data type
                     * \param[in] msg The serialized binary representation of the message
                     */
                    void sendMessage(const SubscriptionID &id, const std::string &msg);

                protected:
                    void handlePacket(const std::string &pkt);
                    void handleError(const spacepi::util::Exception::pointer &err);

                private:
                    MessagingCallback &callback;
                    std::shared_ptr<GenericSocketWrapper> socket;
            };
        }
    }
}

#endif
