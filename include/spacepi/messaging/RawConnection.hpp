#ifndef SPACEPI_CORE_MESSAGING_RAWCONNECTION_HPP
#define SPACEPI_CORE_MESSAGING_RAWCONNECTION_HPP

#include <cstdint>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <boost/asio.hpp>
#include <spacepi/concurrent/ConditionVariable.hpp>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/messaging/network/MessagingCallback.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/messaging/Publisher.hpp>
#include <spacepi/messaging/Subscription.hpp>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>
#include <spacepi/util/Exception.hpp>

namespace spacepi {
    namespace messaging {
        class RawConnection;

        namespace detail {
            class SubscriptionData final {
                public:
                    SubscriptionData() noexcept;

                    SubscriptionData(const SubscriptionData &) = delete;
                    SubscriptionData &operator =(const SubscriptionData &) = delete;

                    void add();
                    bool sub();
                    std::string get();
                    void put(const std::string &msg);

                private:
                    int count;
                    std::queue<std::string> messages;
                    spacepi::concurrent::ConditionVariable cond;
            };

            class ReconnectTimerCallback final {
                public:
                    ReconnectTimerCallback(const std::shared_ptr<RawConnection> &conn) noexcept;

                    void operator ()(const boost::system::error_code &err);

                private:
                    const std::shared_ptr<RawConnection> conn;
            };

            class ConnectionEndpoint final {
                public:
                    enum Type {
                        Invalid,
                        TCP,
                        UNIX
                    };

                    static ConnectionEndpoint defaultEndpoint;

                    ConnectionEndpoint() noexcept;
                    ConnectionEndpoint(const boost::asio::ip::tcp::endpoint &endpoint) noexcept;
                    ConnectionEndpoint(const boost::asio::local::stream_protocol::endpoint &endpoint) noexcept;

                    static bool tryParse(const std::string &str, ConnectionEndpoint &endpoint) noexcept;
                    std::string toString() const noexcept;

                    enum Type getType() const noexcept;
                    const boost::asio::ip::tcp::endpoint &getTCPEndpoint() const noexcept;
                    const boost::asio::local::stream_protocol::endpoint &getUNIXEndpoint() const noexcept;

                private:
                    enum Type type;
                    boost::asio::ip::tcp::endpoint tcpEndpoint;
                    boost::asio::local::stream_protocol::endpoint unixEndpoint;
            };
        }

        /**
         * \brief The implementation behind the Connection class, can be used to override functionality
         */
        class RawConnection : public std::enable_shared_from_this<RawConnection>, public spacepi::util::CommandConfigurable, public spacepi::messaging::network::MessagingCallback, protected spacepi::log::AutoLog<decltype("core:messaging"_autolog)> {
            public:
                /**
                 * \brief Initialize a RawConnection
                 * 
                 * \param[in] cmd The command to use to configure the RawConnection
                 */
                explicit RawConnection(spacepi::util::Command &cmd);

                /**
                 * \brief Destroys a RawConnection
                 */
                virtual ~RawConnection() noexcept;

                RawConnection(RawConnection &) = delete;
                RawConnection &operator =(RawConnection &) = delete;

                /**
                 * \brief Requests that a connection be initiated
                 */
                void connect();

                /**
                 * \brief Starts a publish operation for the given instance ID
                 * 
                 * \param[in] instanceID The instance ID
                 * 
                 * \return The publishing helper (see Publisher::operator<<)
                 */
                Publisher operator ()(uint64_t instanceID);

                /**
                 * \brief Registers a GenericSubscription for this RawConnection
                 * 
                 * \param[in,out] sub The GenericSubscription to register
                 * 
                 * This method is a cancellation point (see spacepi::concurrent::Interrupt).
                 */
                void subscribe(GenericSubscription &sub);

                /**
                 * \brief Unregisters a GenericSubscription from this RawConnection
                 * 
                 * \param[in,out] sub The GenericSubscription to unregister
                 * 
                 * This method is a cancellation point (see spacepi::concurrent::Interrupt).
                 */
                void unsubscribe(GenericSubscription &sub);

                /**
                 * \brief Blocks until data is available for a specified GenericSubscription, and returns it
                 * 
                 * \param[in,out] sub The GenericSubscription to get data for
                 * 
                 * \return The next data packet to be handled by the subscription
                 * 
                 * This method is a cancellation point (see spacepi::concurrent::Interrupt).
                 */
                std::string receive(GenericSubscription &sub);

                /**
                 * \brief Transmits a raw message over the RawConnection
                 * 
                 * \param[in] id The subscription ID for the message
                 * \param[in] msg The message data
                 */
                void send(const network::SubscriptionID &id, const std::string &msg);

                /**
                 * \brief Determines if messages should be printed to the log before publishing
                 * 
                 * \return If they should be
                 */
                bool debugPublishes() const noexcept;

            protected:
                virtual void runCommand();
                virtual void handleMessage(const network::SubscriptionID &id, const std::string &msg);
                virtual void handleConnect();
                virtual void handleError(const spacepi::util::Exception::pointer &err);

            private:
                enum State {
                    Created,
                    Connecting,
                    Connected,
                    Disconnected,
                    Reconnecting,
                    ShutDown
                };

                void updateSubscriptions();

                std::unordered_map<network::SubscriptionID, std::shared_ptr<detail::SubscriptionData>> subscriptions;
                std::unordered_set<network::SubscriptionID> toSubscribe;
                std::unordered_set<network::SubscriptionID> toUnsubscribe;
                std::unique_ptr<spacepi::messaging::network::MessagingSocket> socket;
                enum State state;
                spacepi::concurrent::ConditionVariable cond;
                boost::asio::steady_timer timer;
                detail::ConnectionEndpoint endpoint;
                bool doDebugPublishes;
        };
    }
}

#endif
