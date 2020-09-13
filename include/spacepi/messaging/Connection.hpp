#ifndef SPACEPI_CORE_MESSAGING_CONNECTION_HPP
#define SPACEPI_CORE_MESSAGING_CONNECTION_HPP

#include <condition_variable>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <google/protobuf/message.h>
#include <spacepi/messaging/Subscription.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

namespace spacepi {
    namespace messaging {
        class Connection : public spacepi::util::CommandConfigurable {
            public:
                Connection(std::vector<std::string> &args);
                Connection(const Connection &) = delete;
                ~Connection();

                Connection &operator =(const Connection &) = delete;
                Connection &operator <<(const google::protobuf::Message *message);

                template <typename MessageType, typename std::enable_if<std::is_base_of<google::protobuf::Message, MessageType>::value>::type * = nullptr>
                Connection &operator <<(const MessageType &message) {
                    this << &message;
                    return *this;
                }

                template <typename MessageType, typename std::enable_if<std::is_base_of<google::protobuf::Message, MessageType>::value>::type * = nullptr>
                std::shared_ptr<Subscription<MessageType>> subscribe() {
                    std::shared_ptr<Subscription<MessageType>> sub(new Subscription<MessageType>());
                    subscribe(std::static_pointer_cast<GenericSubscription>(sub));
                    return sub;
                }

                void unsubscribe(uint32_t messageID);
                void waitForMessage(uint32_t messageID, void *ctx, void (*callback)(void *ctx, uint32_t messageID, uint32_t length, void *data));

            protected:
                void options(boost::program_options::options_description &desc) const;
                void configure(const boost::program_options::parsed_options &opts);

            private:
                void subscribe(std::shared_ptr<GenericSubscription> sub);

                boost::asio::io_context ioctx;
                //std::unique_ptr<boost::asio::ip::tcp::socket> tcpSocket;
                //std::unique_ptr<boost::asio::local::stream_protocol::socket> unixSocket;
                std::mutex rxMutex;
                std::condition_variable rxCond;
                std::thread rxThread;
                std::unordered_map<uint32_t, GenericSubscription *> subscriptions;
        };
    }
}

#endif
