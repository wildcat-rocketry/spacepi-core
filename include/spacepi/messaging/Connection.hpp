#ifndef SPACEPI_CORE_MESSAGING_CONNECTION_HPP
#define SPACEPI_CORE_MESSAGING_CONNECTION_HPP

#include <cstddef>
#include <cstdint>
#include <memory>
#include <queue>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <boost/asio.hpp>
#include <boost/fiber/all.hpp>
#include <boost/program_options.hpp>
#include <boost/system/error_code.hpp>
#include <google/protobuf/message.h>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/messaging/network/MessagingCallback.hpp>
#include <spacepi/messaging/network/MessagingSocket.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/util/CommandConfigurable.hpp>
#include <spacepi/util/Exception.hpp>

namespace spacepi {
    namespace messaging {
        class Connection;
        class Publisher;
        class GenericSubscription;

        namespace detail {
            class ImmovableConnection;

            class SubscriptionData {
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
                    boost::fibers::mutex mtx;
                    boost::fibers::condition_variable cond;
            };

            class ReconnectTimerCallback {
                friend class ImmovableConnection;

                public:
                    void operator ()(const boost::system::error_code &err);

                private:
                    ReconnectTimerCallback(ImmovableConnection &conn) noexcept;

                    const std::shared_ptr<ImmovableConnection> conn;
            };

            class ImmovableConnection : public std::enable_shared_from_this<ImmovableConnection>, public spacepi::util::CommandConfigurable, public spacepi::messaging::network::MessagingCallback, private spacepi::log::AutoLog<decltype("core:messaging"_autolog)> {
                friend class messaging::Connection;
                friend class messaging::Publisher;
                friend class ReconnectTimerCallback;

                public:
                    explicit ImmovableConnection(std::vector<std::string> &args);

                    ImmovableConnection(ImmovableConnection &) = delete;
                    ImmovableConnection &operator =(ImmovableConnection &) = delete;

                    Publisher operator ()(uint64_t instanceID);

                    void subscribe(GenericSubscription &sub);
                    void unsubscribe(GenericSubscription &sub);
                    std::string recieve(GenericSubscription &sub);

                protected:
                    void options(boost::program_options::options_description &desc) const;
                    void configure(const boost::program_options::parsed_options &opts);
                    void handleMessage(const network::SubscriptionID &id, const std::string &msg);
                    void handleConnect();
                    void handleError(const spacepi::util::Exception::pointer &err);

                private:
                    enum State {
                        Created,
                        Connecting,
                        Connected,
                        Disconnected,
                        Reconnecting
                    };

                    void connect();
                    void updateSubscriptions();

                    std::unordered_map<network::SubscriptionID, std::shared_ptr<SubscriptionData>> subscriptions;
                    std::unordered_set<network::SubscriptionID> toSubscribe;
                    std::unordered_set<network::SubscriptionID> toUnsubscribe;
                    std::unique_ptr<spacepi::messaging::network::MessagingSocket> socket;
                    enum State state;
                    boost::fibers::mutex mtx;
                    boost::fibers::condition_variable cond;
                    boost::asio::steady_timer timer;
            };
        }

        class Publisher {
            friend class detail::ImmovableConnection;

            public:
                const Publisher &operator <<(const google::protobuf::Message &message) const;
                Publisher &operator <<(const google::protobuf::Message &message);
                
            private:
                Publisher(const std::shared_ptr<detail::ImmovableConnection> &conn, uint64_t instanceID) noexcept;

                const std::shared_ptr<detail::ImmovableConnection> conn;
                const uint64_t instanceID;
        };

        template <typename MessageType, typename std::enable_if<std::is_base_of<google::protobuf::Message, MessageType>::value>::type *>
        class Subscription;

        class Connection {
            template <typename MessageType, typename std::enable_if<std::is_base_of<google::protobuf::Message, MessageType>::value>::type *>
            friend class Subscription;

            public:
                explicit Connection(std::vector<std::string> &args);
                
                Publisher operator ()(uint64_t instanceID) const noexcept;

            private:
                const std::shared_ptr<detail::ImmovableConnection> conn;
        };
    }
}

#endif
