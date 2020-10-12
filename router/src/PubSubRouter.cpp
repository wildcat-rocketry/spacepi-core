#include <condition_variable>
#include <cstdint>
#include <initializer_list>
#include <mutex>
#include <string>
#include <unordered_set>
#include <spacepi/concurrent/RWMutex.hpp>
#include <spacepi/messaging/EncapsulatedMessage.pb.h>
#include <spacepi/messaging/MessageID.pb.h>
#include <spacepi/messaging/SubscribeRequest.pb.h>
#include <spacepi/router/PubSubEndpoint.hpp>
#include <spacepi/router/PubSubRouter.hpp>

using namespace std;
using namespace spacepi::concurrent;
using namespace spacepi::messaging;
using namespace spacepi::router;

PubSubRouter::PubSubRouter() {
}

void PubSubRouter::unregister(PubSubEndpoint *endpoint) {
    unique_lock<RWMutex<mutex, unique_lock<mutex>, condition_variable>::WriteSide> lck(mtx.write());
    fullSubscriptions.erase(endpoint);
    for (unordered_map<uint32_t, unordered_set<PubSubEndpoint *>>::iterator it = subscriptions.begin(); it != subscriptions.end(); ++it) {
        it->second.erase(endpoint);
    }
}

void PubSubRouter::publish(PubSubEndpoint *sender, uint32_t id, const std::string &data) {
    if (id == SubscribeRequest::descriptor()->options().GetExtension(MessageID)) {
        SubscribeRequest req;
        req.ParseFromString(data);
        unique_lock<RWMutex<mutex, unique_lock<mutex>, condition_variable>::WriteSide> lck(mtx.write());
        switch (req.operation()) {
            case SubscribeRequest::OperationType::SubscribeRequest_OperationType_SUBSCRIBE:
                if (fullSubscriptions.find(sender) == fullSubscriptions.end()) {
                    for (int i = req.messages_size() - 1; i >= 0; --i) {
                        uint32_t id = req.messages(i);
                        unordered_map<uint32_t, unordered_set<PubSubEndpoint *>>::iterator it = subscriptions.find(id);
                        if (it == subscriptions.end()) {
                            subscriptions.emplace(id, (initializer_list<PubSubEndpoint *>) {
                                sender
                            });
                        } else {
                            it->second.insert(sender);
                        }
                    }
                }
                break;
            case SubscribeRequest::OperationType::SubscribeRequest_OperationType_UNSUBSCRIBE:
                for (int i = req.messages_size() - 1; i >= 0; --i) {
                    uint32_t id = req.messages(i);
                    unordered_map<uint32_t, unordered_set<PubSubEndpoint *>>::iterator it = subscriptions.find(id);
                    if (it != subscriptions.end()) {
                        it->second.erase(sender);
                    }
                }
                break;
            case SubscribeRequest::OperationType::SubscribeRequest_OperationType_SUBSCRIBE_ALL:
                for (unordered_map<uint32_t, unordered_set<PubSubEndpoint *>>::iterator it = subscriptions.begin(); it != subscriptions.end(); ++it) {
                    it->second.erase(sender);
                }
                fullSubscriptions.insert(sender);
                break;
            case SubscribeRequest::OperationType::SubscribeRequest_OperationType_UNSUBSCRIBE_ALL:
                fullSubscriptions.erase(sender);
                break;
        }
    } else {
        unique_lock<RWMutex<mutex, unique_lock<mutex>, condition_variable>::ReadSide> lck(mtx.read());
        for (unordered_set<PubSubEndpoint *>::iterator it = fullSubscriptions.begin(); it != fullSubscriptions.end(); ++it) {
            if (*it != sender) {
                (*it)->handlePublish(id, data);
            }
        }
        unordered_set<PubSubEndpoint *> &subs = subscriptions[id];
        for (unordered_set<PubSubEndpoint *>::iterator it = subs.begin(); it != subs.end(); ++it) {
            if (*it != sender) {
                (*it)->handlePublish(id, data);
            }
        }
    }
}
