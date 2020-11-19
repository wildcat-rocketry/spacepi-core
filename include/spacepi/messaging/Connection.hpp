#ifndef SPACEPI_CORE_MESSAGING_CONNECTION_HPP
#define SPACEPI_CORE_MESSAGING_CONNECTION_HPP

#include <cstddef>
#include <cstdint>
#include <memory>
#include <queue>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <boost/fiber/all.hpp>
#include <boost/program_options.hpp>
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
                    SubscriptionData(ImmovableConnection &conn, const network::SubscriptionID &id);

                    SubscriptionData(const SubscriptionData &) = delete;
                    SubscriptionData &operator =(const SubscriptionData &) = delete;

                    void add();
                    void sub();
                    std::string get();
                    void put(const std::string &msg);

                private:
                    ImmovableConnection &conn;
                    const network::SubscriptionID id;
                    int count;
                    std::queue<std::string> messages;
                    boost::fibers::mutex mtx;
                    boost::fibers::condition_variable cond;
            };

            class ImmovableConnection : public spacepi::util::CommandConfigurable, public spacepi::messaging::network::MessagingSocket, public spacepi::messaging::network::MessagingCallback, private spacepi::log::AutoLog<decltype("core:messaging"_autolog)> {
                friend class messaging::Connection;
                friend class messaging::Publisher;

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
                    void handleError(const spacepi::util::Exception::pointer &err);

                private:
                    std::unordered_map<network::SubscriptionID, SubscriptionData> subscriptions;
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
