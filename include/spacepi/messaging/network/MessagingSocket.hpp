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

            class MessagingSocket : public SocketWrapperCallback, public std::enable_shared_from_this<MessagingSocket> {
                friend class detail::MessagingSocketAcceptor;
                friend class detail::MessagingSocketConnector;

                public:
                    explicit MessagingSocket(MessagingCallback &callback) noexcept;

                    MessagingSocket(MessagingSocket &) = delete;
                    MessagingSocket &operator =(MessagingSocket &) = delete;

                    template <typename Proto>
                    void accept(typename Proto::acceptor &acceptor);

                    template <typename Proto>
                    void connect(const typename Proto::endpoint &endpoint);

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
