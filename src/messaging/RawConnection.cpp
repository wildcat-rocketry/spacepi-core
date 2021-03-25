#include <chrono>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <spacepi/concurrent/Interrupt.hpp>
#include <spacepi/concurrent/UniqueConditionVariableLock.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/messaging/network/MessagingSocket.hpp>
#include <spacepi/messaging/network/NetworkThread.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/messaging/Publisher.hpp>
#include <spacepi/messaging/RawConnection.hpp>
#include <spacepi/messaging/SubscribeRequest.pb.h>
#include <spacepi/messaging/Subscription.hpp>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>
#include <spacepi/util/CommandInternals.hpp>
#include <spacepi/util/Exception.hpp>

using namespace std;
using namespace std::chrono;
using namespace boost::asio::ip;
using namespace boost::asio::local;
using namespace spacepi::concurrent;
using namespace spacepi::log;
using namespace spacepi::messaging;
using namespace spacepi::messaging::detail;
using namespace spacepi::messaging::network;
using namespace spacepi::util;
using namespace spacepi::util::detail;

ConnectionEndpoint ConnectionEndpoint::defaultEndpoint(stream_protocol::endpoint("/var/run/spacepi.sock"));

SubscriptionData::SubscriptionData() noexcept : count(0) {
}

void SubscriptionData::add() {
    ++count;
}

bool SubscriptionData::sub() {
    return --count == 0;
}

string SubscriptionData::get() {
    UniqueConditionVariableLock lck(cond);
    while (messages.empty()) {
        cond.wait(lck);
    }
    string msg = messages.front();
    messages.pop();
    return msg;
}

void SubscriptionData::put(const string &msg) {
    UniqueConditionVariableLock lck(cond);
    messages.push(msg);
    if (messages.size() == 1) {
        cond.notify_one();
    }
}

ReconnectTimerCallback::ReconnectTimerCallback(const shared_ptr<RawConnection> &conn) noexcept : conn(conn) {
}

void ReconnectTimerCallback::operator ()(const boost::system::error_code &err) {
    conn->connect();
}

ConnectionEndpoint::ConnectionEndpoint() noexcept : type(ConnectionEndpoint::Invalid) {
}

ConnectionEndpoint::ConnectionEndpoint(const tcp::endpoint &endpoint) noexcept : type(ConnectionEndpoint::TCP), tcpEndpoint(endpoint) {
}

ConnectionEndpoint::ConnectionEndpoint(const stream_protocol::endpoint &endpoint) noexcept : type(ConnectionEndpoint::UNIX), unixEndpoint(endpoint) {
}

bool ConnectionEndpoint::tryParse(const string &str, ConnectionEndpoint &endpoint) noexcept {
    try {
        size_t colon = str.find_first_of(':');
        if (colon != string::npos) {
            boost::system::error_code ec;
            address ip = make_address(str.substr(0, colon), ec);
            if (ec) {
                return false;
            }
            size_t len;
            int port = stoi(str.substr(colon + 1), &len);
            if (colon + 1 + len != str.length()) {
                return false;
            }
            if (port < 0 || port > 65535) {
                return false;
            }
            endpoint = ConnectionEndpoint(tcp::endpoint(ip, port));
            return true;
        } else {
            endpoint = ConnectionEndpoint(stream_protocol::endpoint(str));
            return true;
        }
    } catch (const std::exception &) {
        return false;
    }
}

string ConnectionEndpoint::toString() const noexcept {
    switch (type) {
        case TCP:
            return tcpEndpoint.address().to_string() + ":" + to_string(tcpEndpoint.port());
        case UNIX:
            return unixEndpoint.path();
        default:
            return "(invalid)";
    }
}

enum ConnectionEndpoint::Type ConnectionEndpoint::getType() const noexcept {
    return type;
}

const tcp::endpoint &ConnectionEndpoint::getTCPEndpoint() const noexcept {
    return tcpEndpoint;
}

const stream_protocol::endpoint &ConnectionEndpoint::getUNIXEndpoint() const noexcept {
    return unixEndpoint;
}

RawConnection::RawConnection(Command &cmd) : CommandConfigurable("Connection Options", cmd), state(RawConnection::Created), timer(NetworkThread::instance.getContext()) {
    fromCommand(endpoint, ConnectionEndpoint::defaultEndpoint, "router", "The address of the router to connect to");
    fromCommand(doDebugPublishes, false, "debugPublish", "Enable printing all published messages to the debug log");
}

RawConnection::~RawConnection() noexcept {
    UniqueConditionVariableLock lck(cond);
    state = ShutDown;
    cond.notify_all();
}

void RawConnection::connect() {
    UniqueConditionVariableLock lck(cond);
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
    switch (endpoint.getType()) {
        case ConnectionEndpoint::TCP:
            socket->connect<tcp>(endpoint.getTCPEndpoint());
            break;
        case ConnectionEndpoint::UNIX:
            socket->connect<stream_protocol>(endpoint.getUNIXEndpoint());
            break;
        default:
            throw invalid_argument("Unknown endpoint type");
    }
}

Publisher RawConnection::operator ()(uint64_t instanceID) {
    return Publisher(shared_from_this(), instanceID);
}

void RawConnection::subscribe(GenericSubscription &sub) {
    Interrupt::cancellationPoint();
    const network::SubscriptionID &id = sub.getID();
    UniqueConditionVariableLock lck(cond);
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

void RawConnection::unsubscribe(GenericSubscription &sub) {
    Interrupt::cancellationPoint();
    const network::SubscriptionID &id = sub.getID();
    UniqueConditionVariableLock lck(cond);
    unordered_map<network::SubscriptionID, std::shared_ptr<SubscriptionData>>::iterator it = subscriptions.find(id);
    if (it != subscriptions.end() && it->second->sub()) {
        subscriptions.erase(it);
        toSubscribe.erase(id);
        toUnsubscribe.insert(id);
        updateSubscriptions();
    }
}

string RawConnection::receive(GenericSubscription &sub) {
    UniqueConditionVariableLock lck(cond);
    unordered_map<network::SubscriptionID, std::shared_ptr<SubscriptionData>>::iterator it = subscriptions.find(sub.getID());
    if (it != subscriptions.end()) {
        std::shared_ptr<SubscriptionData> ptr = it->second;
        lck.unlock();
        return ptr->get();
    } else {
        throw EXCEPTION(StateException("Cannot receive before subscribing"));
    }
}

void RawConnection::send(const network::SubscriptionID &id, const string &msg) {
    UniqueConditionVariableLock lck(cond);
    socket->sendMessage(id, msg);
}

bool RawConnection::debugPublishes() const noexcept {
    return doDebugPublishes;
}

void RawConnection::runCommand() {
    connect();
    NetworkThread::instance.start();
}

void RawConnection::handleMessage(const network::SubscriptionID &id, const string &msg) {
    UniqueConditionVariableLock lck(cond);
    unordered_map<network::SubscriptionID, std::shared_ptr<SubscriptionData>>::iterator it = subscriptions.find(id);
    if (it != subscriptions.end()) {
        std::shared_ptr<SubscriptionData> ptr = it->second;
        lck.unlock();
        ptr->put(msg);
    }
}

void RawConnection::handleConnect() {
    log(LogLevel::Info) << "Connected to router.";
    UniqueConditionVariableLock lck(cond);
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
        case ShutDown:
            return;
        default:
            throw EXCEPTION(StateException("Invalid state for socket connect to occur"));
    }
    state = Connected;
    updateSubscriptions();
    cond.notify_all();
}

void RawConnection::handleError(const Exception::pointer &err) {
    UniqueConditionVariableLock lck(cond);
    switch (state) {
        case Connecting:
            log(LogLevel::Warning) << "Error connecting to router: " << Exception::what(err) << "\n"
                                      "Will attempt to retry connection...";
            break;
        case Connected:
            log(LogLevel::Warning) << "Lost connection from router.\n"
                                      "Will attempt to reconnect...";
            break;
        case Reconnecting:
            log(LogLevel::Debug) << "Connection attempt failed.";
            break;
        case ShutDown:
            return;
        default:
            throw EXCEPTION(StateException("Invalid state for socket error to occur"));
    }
    state = Disconnected;
    lck.unlock();
    timer.expires_after(seconds(1));
    timer.async_wait(ReconnectTimerCallback(shared_from_this()));
}

void RawConnection::updateSubscriptions() {
    if (state == Connected) {
        SubscribeRequest req;
        for (unordered_set<network::SubscriptionID>::const_iterator it = toSubscribe.begin(); it != toSubscribe.end(); ++it) {
            detail::SubscriptionID *sub = req.add_subscribe();
            sub->set_messageid(it->getMessageID());
            sub->set_instanceid(it->getInstanceID());
        }
        toSubscribe.clear();
        for (unordered_set<network::SubscriptionID>::const_iterator it = toUnsubscribe.begin(); it != toUnsubscribe.end(); ++it) {
            detail::SubscriptionID *sub = req.add_unsubscribe();
            sub->set_messageid(it->getMessageID());
            sub->set_instanceid(it->getInstanceID());
        }
        toUnsubscribe.clear();
        socket->sendMessage(network::SubscriptionID(req.GetDescriptor()->options().GetExtension(MessageID), 0), req.SerializeAsString());
    }
}

template <>
pair<vector<string>::const_iterator, string> CommandParser<ConnectionEndpoint>::parse(const vector<string> &args, const vector<string>::const_iterator &start) noexcept {
    pair<string, pair<vector<string>::const_iterator, string>> res = parseNormal(args, start);
    if (!res.first.empty()) {
        valid = ConnectionEndpoint::tryParse(res.first, var);
        if (!valid) {
            return make_pair(res.second.first, "Invalid endpoint format");
        }
    }
    return res.second;
}

template <>
string CommandParser<ConnectionEndpoint>::example() const noexcept {
    return "--" + name + "=" + ConnectionEndpoint::defaultEndpoint.toString();
}
