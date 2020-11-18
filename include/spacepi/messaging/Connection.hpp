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
#include <spacepi/messaging/Subscription.hpp>
#include <spacepi/messaging/network/MessagingSocket.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/util/CommandConfigurable.hpp>
#include <spacepi/util/Exception.hpp>

namespace spacepi {
    namespace messaging {
        class ImmovableConnection;

        class SubscriptionData {
            public:
                SubscriptionData(ImmovableConnection *conn, const network::SubscriptionID &id);
                SubscriptionData(const SubscriptionData &) = delete;

                SubscriptionData &operator =(const SubscriptionData &) = delete;

                void add();
                void sub();
                std::string get();
                void put(const std::string &msg);

            private:
                ImmovableConnection *conn;
                network::SubscriptionID id;
                int count;
                std::queue<std::string> messages;
                boost::fibers::mutex mtx;
                boost::fibers::condition_variable cond;
        };

        class Publisher {
            friend class ImmovableConnection;

            public:
                Publisher &operator <<(const google::protobuf::Message *message);
                
                template <typename MessageType, typename std::enable_if<std::is_base_of<google::protobuf::Message, MessageType>::value>::type * = nullptr>
                Publisher &operator <<(const MessageType &message) {
                    *this << (const google::protobuf::Message *) &message;
                    return *this;
                }

            private:
                Publisher(std::shared_ptr<ImmovableConnection> conn, uint64_t instanceID);

                std::shared_ptr<ImmovableConnection> conn;
                uint64_t instanceID;
        };

        class Connection;

        class ImmovableConnection : public spacepi::util::CommandConfigurable, public std::enable_shared_from_this<ImmovableConnection>, private spacepi::messaging::network::MessagingSocket, public spacepi::messaging::network::MessagingCallback, private spacepi::log::AutoLog<decltype("core:messaging"_autolog)> {
            friend class Connection;
            friend class Publisher;

            public:
                ImmovableConnection(std::vector<std::string> &args);
                ImmovableConnection(const ImmovableConnection &) = delete;

                ImmovableConnection &operator =(const ImmovableConnection &) = delete;

                Publisher operator ()(uint64_t instanceID);

                void subscribe(GenericSubscription *sub);
                void unsubscribe(GenericSubscription *sub);
                std::string recieve(GenericSubscription *sub);

            protected:
                void options(boost::program_options::options_description &desc) const;
                void configure(const boost::program_options::parsed_options &opts);
                void handleMessage(const network::SubscriptionID &id, const std::string &msg);
                void handleError(spacepi::util::Exception::pointer err);

            private:
                std::unordered_map<network::SubscriptionID, SubscriptionData> subscriptions;
        };

        class Connection {
            public:
                Connection(std::vector<std::string> &args);
                
                operator ImmovableConnection *();
                Publisher operator ()(uint64_t instanceID);

            private:
                std::shared_ptr<ImmovableConnection> conn;
        };
    }
}

#endif
