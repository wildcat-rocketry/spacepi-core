#ifndef SPACEPI_CORE_MESSAGING_NETWORK_MESSAGINGSOCKET_HPP
#define SPACEPI_CORE_MESSAGING_NETWORK_MESSAGINGSOCKET_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/messaging/network/SocketWrapper.hpp>
#include <spacepi/util/Exception.hpp>

namespace spacepi {
    namespace messaging {
        namespace network {
            class MessagingSocket;
            class MessagingSocketAcceptor;
            class MessagingSocketConnector;

            class MessagingCallback {
                friend class MessagingSocket;
                friend class MessagingSocketAcceptor;
                friend class MessagingSocketConnector;

                protected:
                    virtual void handleMessage(uint32_t id, const std::string &msg) = 0;
                    virtual void handleAccept();
                    virtual void handleConnect();
                    virtual void handleError(spacepi::util::Exception::pointer err) = 0;
            };

            class DefaultMessagingCallback : public MessagingCallback, private spacepi::log::AutoLog<decltype("core:messaging"_autolog)> {
                protected:
                    virtual void handleError(spacepi::util::Exception::pointer err);
            };

            class MessagingSocketAcceptor {
                friend class MessagingSocket;

                public:
                    void operator ()(const boost::system::error_code &err);

                private:
                    MessagingSocketAcceptor(MessagingSocket *socket);

                    MessagingSocket *socket;
            };

            class MessagingSocketConnector {
                friend class MessagingSocket;

                public:
                    void operator ()(const boost::system::error_code &err);

                private:
                    MessagingSocketConnector(MessagingSocket *socket);

                    MessagingSocket *socket;
            };

            class MessagingSocket : public SocketWrapperCallback {
                friend class MessagingSocketAcceptor;
                friend class MessagingSocketConnector;

                public:
                    MessagingSocket(MessagingCallback *callback);

                    template <typename Acceptor>
                    void accept(Acceptor &acceptor);

                    template <typename Endpoint>
                    void connect(const Endpoint &endpoint);

                    void sendMessage(uint32_t id, const std::string &msg);

                protected:
                    void handlePacket(const std::string &pkt);
                    void handleError(spacepi::util::Exception::pointer err);

                private:
                    MessagingCallback *callback;
                    std::shared_ptr<GenericSocketWrapper> socket;
                    MessagingSocketAcceptor acceptor;
                    MessagingSocketConnector connector;
            };
        }
    }
}

#endif
