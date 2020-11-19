#include <chrono>
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
#include <spacepi/messaging/network/NetworkThread.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/util/CommandConfigurable.hpp>
#include <spacepi/util/Exception.hpp>

using namespace std;
using namespace std::chrono;
using namespace boost;
using namespace boost::asio::ip;
using namespace boost::program_options;
using namespace google::protobuf;
using namespace spacepi;
using namespace spacepi::messaging;
using namespace spacepi::messaging::detail;
using namespace spacepi::messaging::network;
using namespace spacepi::util;

SubscriptionData::SubscriptionData(ImmovableConnection &conn, const SubscriptionID &id) : conn(conn), id(id), count(0) {
}

void SubscriptionData::add() {
    std::unique_lock<fibers::mutex> lck(mtx);
    if (++count == 1) {
        SubscribeRequest req;
        req.set_messageid(id.getMessageID());
        req.set_operation(SubscribeRequest_OperationType::SubscribeRequest_OperationType_SUBSCRIBE);
        conn(id.getInstanceID()) << req;
    }
}

void SubscriptionData::sub() {
    std::unique_lock<fibers::mutex> lck(mtx);
    if (--count == 0) {
        SubscribeRequest req;
        req.set_messageid(id.getMessageID());
        req.set_operation(SubscribeRequest_OperationType::SubscribeRequest_OperationType_UNSUBSCRIBE);
        conn(id.getInstanceID()) << req;
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

void ReconnectTimerCallback::operator ()(const system::error_code &err) {
    conn->connect();
}

ReconnectTimerCallback::ReconnectTimerCallback(ImmovableConnection &conn) noexcept : conn(conn.shared_from_this()) {
}

ImmovableConnection::ImmovableConnection(vector<string> &args) : CommandConfigurable("Connection Options", args), state(ImmovableConnection::Created), timer(NetworkThread::instance.getContext()) {
    construct();
}

Publisher ImmovableConnection::operator ()(uint64_t instanceID) {
    return Publisher(shared_from_this(), instanceID);
}

void ImmovableConnection::subscribe(GenericSubscription &sub) {
    const SubscriptionID &id = sub.getID();
    subscriptions.emplace(piecewise_construct, std::make_tuple(id), std::tuple<ImmovableConnection &, const SubscriptionID &>(*this, id)).first->second.add();
}

void ImmovableConnection::unsubscribe(GenericSubscription &sub) {
    const SubscriptionID &id = sub.getID();
    subscriptions.emplace(piecewise_construct, std::make_tuple(id), std::tuple<ImmovableConnection &, const SubscriptionID &>(*this, id)).first->second.sub();
}

string ImmovableConnection::recieve(GenericSubscription &sub) {
    const SubscriptionID &id = sub.getID();
    return subscriptions.emplace(piecewise_construct, std::make_tuple(id), std::tuple<ImmovableConnection &, const SubscriptionID &>(*this, id)).first->second.get();
}

void ImmovableConnection::options(options_description &desc) const {
    // TODO
}

void ImmovableConnection::configure(const parsed_options &opts) {
    // TODO
    connect();
}

void ImmovableConnection::handleMessage(const SubscriptionID &id, const std::string &msg) {
    unordered_map<SubscriptionID, SubscriptionData>::iterator it = subscriptions.find(id);
    if (it != subscriptions.end()) {
        it->second.put(msg);
    }
}

void ImmovableConnection::handleConnect() {
    log(log::LogLevel::Info) << "Connected to router.";
    std::unique_lock<fibers::mutex> lck(mtx);
    state = Connected;
    cond.notify_all();
}

void ImmovableConnection::handleError(const Exception::pointer &err) {
    std::unique_lock<fibers::mutex> lck(mtx);
    switch (state) {
        case Connecting:
            log(log::LogLevel::Warning) << "Error connecting to router: " << Exception::what(err) << "\n"
                                           "Will attempt to retry connection...";
            break;
        case Connected:
            log(log::LogLevel::Warning) << "Lost connection from router.\n"
                                           "Will attempt to reconnect...";
            break;
        case Reconnecting:
            log(log::LogLevel::Debug) << "Connection attempt failed.";
            break;
        default:
            throw EXCEPTION(StateException("Invalid state for socket error to occur"));
    }
    state = Disconnected;
    lck.unlock();
    timer.expires_after(seconds(1));
    timer.async_wait(ReconnectTimerCallback(*this));
}

void ImmovableConnection::connect() {
    std::unique_lock<fibers::mutex> lck(mtx);
    switch (state) {
        case Created:
            state = Connecting;
            break;
        case Disconnected:
            state = Reconnecting;
            break;
        default:
            return;
    }
    lck.unlock();
    socket.reset(new MessagingSocket(*this));
    socket->connect<tcp>(tcp::endpoint(address::from_string("127.0.0.1"), 8000));
}

const Publisher &Publisher::operator <<(const Message &message) const {
    std::unique_lock<fibers::mutex> lck(conn->mtx);
    while (conn->state != ImmovableConnection::Connected) {
        conn->cond.wait(lck);
    }
    lck.unlock();
    conn->socket->sendMessage(SubscriptionID(message.GetDescriptor()->options().GetExtension(MessageID), instanceID), message.SerializeAsString());
    return *this;
}

Publisher &Publisher::operator <<(const Message &message) {
    ((const Publisher &) *this) << message;
    return *this;
}

Publisher::Publisher(const std::shared_ptr<ImmovableConnection> &conn, uint64_t instanceID) noexcept : conn(conn), instanceID(instanceID) {
}

Connection::Connection(vector<string> &args) : conn(new ImmovableConnection(args)) {
}

Publisher Connection::operator ()(uint64_t instanceID) const noexcept {
    return (*conn)(instanceID);
}
