#include <functional>
#include <string>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/messaging/RawConnection.hpp>
#include <spacepi/messaging/SubscribeRequest.pb.h>
#include <spacepi/util/Command.hpp>
#include <spacepi/journaler/FullConnection.hpp>

using namespace std;
using namespace spacepi::messaging;
using namespace spacepi::messaging::detail;
using namespace spacepi::util;
using namespace spacepi::journaler;

FullConnection::FullConnection(Command &cmd) noexcept : RawConnection(cmd) {
}

void FullConnection::subscribeAll(const function<void(const network::SubscriptionID &, const string &)> &func) {
    subscribeFunc = func;
    resubscribe();
}

void FullConnection::handleMessage(const network::SubscriptionID &id, const string &msg) {
    if (subscribeFunc) {
        subscribeFunc(id, msg);
    }
}

void FullConnection::handleConnect() {
    RawConnection::handleConnect();
    resubscribe();
}

void FullConnection::resubscribe() {
    SubscribeRequest req;
    if (subscribeFunc) {
        req.set_subscribeall(true);
    } else {
        req.set_unsubscribeall(true);
    }
    (*this)(0) << req;
}
