#include <condition_variable>
#include <initializer_list>
#include <mutex>
#include <string>
#include <unordered_set>
#include <spacepi/concurrent/RWMutex.hpp>
#include <spacepi/messaging/EncapsulatedMessage.pb.h>
#include <spacepi/messaging/MessageID.pb.h>
#include <spacepi/messaging/SubscribeRequest.pb.h>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/router/PubSubEndpoint.hpp>
#include <spacepi/router/PubSubRouter.hpp>

using namespace std;
using namespace spacepi::concurrent;
using namespace spacepi::messaging;
using namespace spacepi::messaging::network;
using namespace spacepi::router;

void PubSubRouter::release(PubSubEndpoint &sender) noexcept {
    unique_lock<RWMutex<mutex, unique_lock<mutex>, condition_variable>::WriteSide> lck(mtx.write());
    fullSubscriptions.erase(&sender);
    for (unordered_map<SubscriptionID, unordered_set<PubSubEndpoint *>>::iterator it = subscriptions.begin(); it != subscriptions.end(); ++it) {
        it->second.erase(&sender);
    }
}

void PubSubRouter::publish(PubSubEndpoint &sender, const SubscriptionID &id, const std::string &data) {
    if (id.getMessageID() == SubscribeRequest::descriptor()->options().GetExtension(MessageID)) {
        SubscribeRequest req;
        req.ParseFromString(data);
        unique_lock<RWMutex<mutex, unique_lock<mutex>, condition_variable>::WriteSide> lck(mtx.write());
        SubscriptionID chanId(req.messageid(), id.getInstanceID());
        unordered_map<SubscriptionID, unordered_set<PubSubEndpoint *>>::iterator it;
        switch (req.operation()) {
            case SubscribeRequest::OperationType::SubscribeRequest_OperationType_SUBSCRIBE:
                if (fullSubscriptions.find(&sender) == fullSubscriptions.end()) {
                    it = subscriptions.find(chanId);
                    if (it == subscriptions.end()) {
                        subscriptions.emplace_hint(it, chanId, (initializer_list<PubSubEndpoint *>) {
                            &sender
                        });
                    } else {
                        it->second.insert(&sender);
                    }
                }
                break;
            case SubscribeRequest::OperationType::SubscribeRequest_OperationType_UNSUBSCRIBE:
                it = subscriptions.find(chanId);
                if (it != subscriptions.end()) {
                    it->second.erase(&sender);
                }
                break;
            case SubscribeRequest::OperationType::SubscribeRequest_OperationType_SUBSCRIBE_ALL:
                for (it = subscriptions.begin(); it != subscriptions.end(); ++it) {
                    it->second.erase(&sender);
                }
                fullSubscriptions.insert(&sender);
                break;
            case SubscribeRequest::OperationType::SubscribeRequest_OperationType_UNSUBSCRIBE_ALL:
                fullSubscriptions.erase(&sender);
                break;
        }
    } else {
        unique_lock<RWMutex<mutex, unique_lock<mutex>, condition_variable>::ReadSide> lck(mtx.read());
        publish(sender, id, data, fullSubscriptions);
        publish(sender, id, data, subscriptions[id]);
    }
}

void PubSubRouter::publish(PubSubEndpoint &sender, const SubscriptionID &id, const std::string &data, unordered_set<PubSubEndpoint *> &endpoints) {
    for (unordered_set<PubSubEndpoint *>::iterator it = endpoints.begin(); it != endpoints.end(); ++it) {
        (*it)->handlePublish(id, data);
    }
}
