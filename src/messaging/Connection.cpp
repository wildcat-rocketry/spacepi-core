#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <boost/asio.hpp>
#include <boost/fiber/all.hpp>
#include <boost/program_options.hpp>
#include <boost/system/error_code.hpp>
#include <google/protobuf/message.h>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/messaging/Connection.hpp>
#include <spacepi/messaging/MessageID.pb.h>
#include <spacepi/messaging/SubscribeRequest.pb.h>
#include <spacepi/messaging/Subscription.hpp>
#include <spacepi/messaging/SubscriptionID.pb.h>
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

SubscriptionData::SubscriptionData() noexcept : count(0) {
}

void SubscriptionData::add() {
    ++count;
}

bool SubscriptionData::sub() {
    return --count == 0;
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
    const network::SubscriptionID &id = sub.getID();
    std::unique_lock<fibers::mutex> lck(mtx);
    unordered_map<network::SubscriptionID, std::shared_ptr<SubscriptionData>>::iterator it = subscriptions.find(id);
    if (it == subscriptions.end()) {
        std::shared_ptr<SubscriptionData> ptr(new SubscriptionData());
        it = subscriptions.emplace_hint(it, piecewise_construct, std::make_tuple(id), std::make_tuple(std::move(ptr)));
        toSubscribe.insert(id);
        toUnsubscribe.erase(id);
        updateSubscriptions();
    }
    it->second->add();
}

void ImmovableConnection::unsubscribe(GenericSubscription &sub) {
    const network::SubscriptionID &id = sub.getID();
    std::unique_lock<fibers::mutex> lck(mtx);
    unordered_map<network::SubscriptionID, std::shared_ptr<SubscriptionData>>::iterator it = subscriptions.find(id);
    if (it != subscriptions.end() && it->second->sub()) {
        subscriptions.erase(it);
        toSubscribe.erase(id);
        toUnsubscribe.insert(id);
        updateSubscriptions();
    }
}

string ImmovableConnection::recieve(GenericSubscription &sub) {
    std::unique_lock<fibers::mutex> lck(mtx);
    unordered_map<network::SubscriptionID, std::shared_ptr<SubscriptionData>>::iterator it = subscriptions.find(sub.getID());
    if (it != subscriptions.end()) {
        std::shared_ptr<SubscriptionData> ptr = it->second;
        lck.unlock();
        return ptr->get();
    } else {
        throw EXCEPTION(StateException("Cannot receive before subscribing"));
    }
}

void ImmovableConnection::options(options_description &desc) const {
    // TODO
}

void ImmovableConnection::configure(const parsed_options &opts) {
    // TODO
    connect();
}

void ImmovableConnection::handleMessage(const network::SubscriptionID &id, const std::string &msg) {
    std::unique_lock<fibers::mutex> lck(mtx);
    unordered_map<network::SubscriptionID, std::shared_ptr<SubscriptionData>>::iterator it = subscriptions.find(id);
    if (it != subscriptions.end()) {
        std::shared_ptr<SubscriptionData> ptr = it->second;
        lck.unlock();
        ptr->put(msg);
    }
}

void ImmovableConnection::handleConnect() {
    log(log::LogLevel::Info) << "Connected to router.";
    std::unique_lock<fibers::mutex> lck(mtx);
    SubscribeRequest req;
    switch (state) {
        case Connecting:
            break;
        case Reconnecting:
            toUnsubscribe.clear();
            for (unordered_map<network::SubscriptionID, std::shared_ptr<SubscriptionData>>::const_iterator it = subscriptions.begin(); it != subscriptions.end(); ++it) {
                toSubscribe.insert(it->first);
            }
            break;
        default:
            throw EXCEPTION(StateException("Invalid state for socket connect to occur"));
    }
    state = Connected;
    updateSubscriptions();
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

void ImmovableConnection::updateSubscriptions() {
    if (state == Connected) {
        SubscribeRequest req;
        for (unordered_set<network::SubscriptionID>::const_iterator it = toSubscribe.begin(); it != toSubscribe.end(); ++it) {
            messaging::SubscriptionID *sub = req.add_subscribe();
            sub->set_messageid(it->getMessageID());
            sub->set_instanceid(it->getInstanceID());
        }
        toSubscribe.clear();
        for (unordered_set<network::SubscriptionID>::const_iterator it = toUnsubscribe.begin(); it != toUnsubscribe.end(); ++it) {
            messaging::SubscriptionID *sub = req.add_unsubscribe();
            sub->set_messageid(it->getMessageID());
            sub->set_instanceid(it->getInstanceID());
        }
        toUnsubscribe.clear();
        socket->sendMessage(network::SubscriptionID(req.GetDescriptor()->options().GetExtension(MessageID), 0), req.SerializeAsString());
    }
}

const Publisher &Publisher::operator <<(const Message &message) const {
    std::unique_lock<fibers::mutex> lck(conn->mtx);
    while (conn->state != ImmovableConnection::Connected) {
        conn->cond.wait(lck);
    }
    conn->socket->sendMessage(network::SubscriptionID(message.GetDescriptor()->options().GetExtension(MessageID), instanceID), message.SerializeAsString());
    lck.unlock();
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
