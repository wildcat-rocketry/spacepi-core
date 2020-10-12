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
#include <spacepi/util/CommandConfigurable.hpp>
#include <spacepi/util/Exception.hpp>
#include <spacepi/util/Functional.hpp>

namespace spacepi {
    namespace messaging {
        class ImmovableConnection;

        class SubscriptionData {
            public:
                SubscriptionData(ImmovableConnection *conn, uint32_t id);
                SubscriptionData(const SubscriptionData &) = delete;

                SubscriptionData &operator =(const SubscriptionData &) = delete;

                void add();
                void sub();
                std::string get();
                void put(const std::string &msg);

            private:
                ImmovableConnection *conn;
                uint32_t id;
                int count;
                std::queue<std::string> messages;
                boost::fibers::mutex mtx;
                boost::fibers::condition_variable cond;
        };

        class Connection;

        class ImmovableConnection : public spacepi::util::CommandConfigurable, private spacepi::messaging::network::MessagingSocket, public spacepi::messaging::network::MessagingCallback, private spacepi::log::AutoLog<decltype("core:messaging"_autolog)> {
            friend class Connection;

            public:
                ImmovableConnection(std::vector<std::string> &args);
                ImmovableConnection(const ImmovableConnection &) = delete;

                ImmovableConnection &operator =(const ImmovableConnection &) = delete;
                ImmovableConnection &operator <<(const google::protobuf::Message *message);

                template <typename MessageType, typename std::enable_if<std::is_base_of<google::protobuf::Message, MessageType>::value>::type * = nullptr>
                ImmovableConnection &operator <<(const MessageType &message) {
                    *this << (const google::protobuf::Message *) &message;
                    return *this;
                }

                void subscribe(GenericSubscription *sub);
                void unsubscribe(GenericSubscription *sub);
                std::string recieve(GenericSubscription *sub);

            protected:
                void options(boost::program_options::options_description &desc) const;
                void configure(const boost::program_options::parsed_options &opts);
                void handleMessage(uint32_t id, const std::string &msg);
                void handleError(spacepi::util::Exception::pointer err);

            private:
                std::unordered_map<uint32_t, SubscriptionData, spacepi::util::Cast<size_t, uint32_t>> subscriptions;
        };

        class Connection {
            public:
                Connection(std::vector<std::string> &args);

                Connection &operator <<(const google::protobuf::Message *message);
                operator ImmovableConnection *();

                template <typename MessageType, typename std::enable_if<std::is_base_of<google::protobuf::Message, MessageType>::value>::type * = nullptr>
                Connection &operator <<(const MessageType &message) {
                    *this << (const google::protobuf::Message *) &message;
                    return *this;
                }

            private:
                std::shared_ptr<ImmovableConnection> conn;
        };
    }
}

#endif
