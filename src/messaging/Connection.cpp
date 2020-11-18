#include <cstddef>
#include <cstdint>
#include <mutex>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <unordered_map>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/system/error_code.hpp>
#include <google/protobuf/message.h>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/messaging/Connection.hpp>
#include <spacepi/messaging/MessageID.pb.h>
#include <spacepi/messaging/SubscribeRequest.pb.h>
#include <spacepi/messaging/Subscription.hpp>
#include <spacepi/messaging/network/MessagingSocket.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

using namespace std;
using namespace boost;
using namespace boost::asio::ip;
using namespace boost::program_options;
using namespace google::protobuf;
using namespace spacepi;
using namespace spacepi::messaging;
using namespace spacepi::messaging::network;
using namespace spacepi::util;

SubscriptionData::SubscriptionData(ImmovableConnection *conn, const SubscriptionID &id) : conn(conn), id(id), count(0) {
}

void SubscriptionData::add() {
    std::unique_lock<fibers::mutex> lck(mtx);
    if (++count == 1) {
        SubscribeRequest req;
        req.set_messageid(id.getMessageID());
        req.set_operation(SubscribeRequest_OperationType::SubscribeRequest_OperationType_SUBSCRIBE);
        (*conn)(id.getInstanceID()) << req;
    }
}

void SubscriptionData::sub() {
    std::unique_lock<fibers::mutex> lck(mtx);
    if (--count == 0) {
        SubscribeRequest req;
        req.set_messageid(id.getMessageID());
        req.set_operation(SubscribeRequest_OperationType::SubscribeRequest_OperationType_UNSUBSCRIBE);
        (*conn)(id.getInstanceID()) << req;
    }
}

string SubscriptionData::get() {
    std::unique_lock<fibers::mutex> lck(mtx);
    while (messages.empty()) {
        cond.wait(lck);
    }
    string msg = messages.front();
    messages.pop();
    return msg;
}

void SubscriptionData::put(const string &msg) {
    std::unique_lock<fibers::mutex> lck(mtx);
    messages.push(msg);
    if (messages.size() == 1) {
        cond.notify_one();
    }
}

Publisher &Publisher::operator <<(const Message *message) {
    conn->sendMessage(SubscriptionID(message->GetDescriptor()->options().GetExtension(MessageID), instanceID), message->SerializeAsString());
    return *this;
}

Publisher::Publisher(std::shared_ptr<ImmovableConnection> conn, uint64_t instanceID) : conn(conn), instanceID(instanceID) {
}

ImmovableConnection::ImmovableConnection(vector<string> &args) : CommandConfigurable("Connection Options", args), MessagingSocket(this) {
    construct();
}

Publisher ImmovableConnection::operator ()(uint64_t instanceID) {
    return Publisher(shared_from_this(), instanceID);
}

void ImmovableConnection::subscribe(GenericSubscription *sub) {
    const SubscriptionID &id = sub->getID();
    subscriptions.emplace(piecewise_construct, std::make_tuple(id), std::make_tuple(this, id)).first->second.add();
}

void ImmovableConnection::unsubscribe(GenericSubscription *sub) {
    const SubscriptionID &id = sub->getID();
    subscriptions.emplace(piecewise_construct, std::make_tuple(id), std::make_tuple(this, id)).first->second.sub();
}

string ImmovableConnection::recieve(GenericSubscription *sub) {
    const SubscriptionID &id = sub->getID();
    return subscriptions.emplace(piecewise_construct, std::make_tuple(id), std::make_tuple(this, id)).first->second.get();
}

void ImmovableConnection::options(options_description &desc) const {
    // TODO
}

void ImmovableConnection::configure(const parsed_options &opts) {
    // TODO
    connect(tcp::endpoint(address::from_string("127.0.0.1"), 8000));
}

void ImmovableConnection::handleMessage(const SubscriptionID &id, const std::string &msg) {
    unordered_map<SubscriptionID, SubscriptionData>::iterator it = subscriptions.find(id);
    if (it != subscriptions.end()) {
        it->second.put(msg);
    }
}

void ImmovableConnection::handleError(Exception::pointer err) {
    log(log::LogLevel::Error) << "Connection error: " << err;
}

Connection::Connection(vector<string> &args) : conn(new ImmovableConnection(args)) {
}

Connection::operator ImmovableConnection *() {
    return conn.get();
}

Publisher Connection::operator ()(uint64_t instanceID) {
    return (*conn)(instanceID);
}
