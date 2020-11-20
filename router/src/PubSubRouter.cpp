#include <condition_variable>
#include <initializer_list>
#include <mutex>
#include <string>
#include <unordered_set>
#include <google/protobuf/repeated_field.h>
#include <spacepi/concurrent/RWMutex.hpp>
#include <spacepi/messaging/EncapsulatedMessage.pb.h>
#include <spacepi/messaging/MessageID.pb.h>
#include <spacepi/messaging/SubscribeRequest.pb.h>
#include <spacepi/messaging/SubscriptionID.pb.h>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/router/PubSubEndpoint.hpp>
#include <spacepi/router/PubSubRouter.hpp>

using namespace std;
using namespace google::protobuf;
using namespace spacepi;
using namespace spacepi::concurrent;
using namespace spacepi::messaging;
using namespace spacepi::messaging::network;
using namespace spacepi::router;

void PubSubRouter::release(PubSubEndpoint &sender) noexcept {
    unique_lock<RWMutex<mutex, unique_lock<mutex>, condition_variable>::WriteSide> lck(mtx.write());
    fullSubscriptions.erase(&sender);
    for (unordered_map<network::SubscriptionID, unordered_set<PubSubEndpoint *>>::iterator it = subscriptions.begin(); it != subscriptions.end(); ++it) {
        it->second.erase(&sender);
    }
}

void PubSubRouter::publish(PubSubEndpoint &sender, const network::SubscriptionID &id, const std::string &data) {
    if (id.getMessageID() == SubscribeRequest::descriptor()->options().GetExtension(MessageID)) {
        SubscribeRequest req;
        req.ParseFromString(data);
        unique_lock<RWMutex<mutex, unique_lock<mutex>, condition_variable>::WriteSide> lck(mtx.write());
        unordered_set<PubSubEndpoint *>::iterator fullSub = fullSubscriptions.find(&sender);
        if (fullSub != fullSubscriptions.end() && req.unsubscribeall()) {
            fullSubscriptions.erase(fullSub);
            fullSub = fullSubscriptions.end();
        }
        if (fullSub == fullSubscriptions.end()) {
            if (req.subscribeall()) {
                for (unordered_map<network::SubscriptionID, unordered_set<PubSubEndpoint *>>::iterator it = subscriptions.begin(); it != subscriptions.end(); ++it) {
                    it->second.erase(&sender);
                }
                fullSubscriptions.insert(&sender);
            } else {
                const RepeatedPtrField<messaging::SubscriptionID> &unsubs = req.unsubscribe();
                for (RepeatedPtrField<messaging::SubscriptionID>::const_iterator it = unsubs.begin(); it != unsubs.end(); ++it) {
                    network::SubscriptionID chanId(it->messageid(), it->instanceid());
                    unordered_map<network::SubscriptionID, unordered_set<PubSubEndpoint *>>::iterator chan = subscriptions.find(chanId);
                    if (chan != subscriptions.end()) {
                        chan->second.erase(&sender);
                    }
                }
                const RepeatedPtrField<messaging::SubscriptionID> &subs = req.subscribe();
                for (RepeatedPtrField<messaging::SubscriptionID>::const_iterator it = subs.begin(); it != subs.end(); ++it) {
                    network::SubscriptionID chanId(it->messageid(), it->instanceid());
                    pair<unordered_map<network::SubscriptionID, unordered_set<PubSubEndpoint *>>::iterator, bool> res = subscriptions.emplace(chanId, (initializer_list<PubSubEndpoint *>) { &sender });
                    if (!res.second) {
                        res.first->second.insert(&sender);
                    }
                }
            }
        }
    } else {
        unique_lock<RWMutex<mutex, unique_lock<mutex>, condition_variable>::ReadSide> lck(mtx.read());
        publish(sender, id, data, fullSubscriptions);
        publish(sender, id, data, subscriptions[id]);
    }
}

void PubSubRouter::publish(PubSubEndpoint &sender, const network::SubscriptionID &id, const std::string &data, unordered_set<PubSubEndpoint *> &endpoints) {
    for (unordered_set<PubSubEndpoint *>::iterator it = endpoints.begin(); it != endpoints.end(); ++it) {
        if (*it != &sender) {
            (*it)->handlePublish(id, data);
        }
    }
}
