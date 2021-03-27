#include <chrono>
#include <exception>
#include <functional>
#include <string>
#include <spacepi/concurrent/UniqueConditionVariableLock.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/messaging/JournalEntry.pb.h>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>
#include <spacepi/journaler/FullConnection.hpp>
#include <spacepi/journaler/Journal.hpp>
#include <spacepi/journaler/Journaler.hpp>

using namespace std;
using namespace std::chrono;
using namespace std::placeholders;
using namespace spacepi::concurrent;
using namespace spacepi::messaging;
using namespace spacepi::messaging::detail;
using namespace spacepi::util;
using namespace spacepi::journaler;

Journaler::Journaler(Command &cmd, Journal &journal, FullConnection &conn) noexcept : CommandConfigurable("Journaling Options", cmd), journal(journal), conn(conn), enable(false) {
    fromCommand(enable, false, "journal", "Capture a journal of all messages sent over the messaging bus");
}

void Journaler::handleMessage(const network::SubscriptionID &id, const string &msg) {
    steady_clock::time_point now = steady_clock::now();
    JournalEntry ent;
    ent.set_timestamp(nanoseconds(now - start).count());
    ent.set_messageid(id.getMessageID());
    ent.set_instanceid(id.getInstanceID());
    ent.set_payload(msg);
    UniqueConditionVariableLock lck(cond);
    queue.push(ent);
    cond.notify_one();
}

void Journaler::runCommand() {
    if (!enable) {
        return;
    }
    UniqueConditionVariableLock lck(cond);
    journal.open(true);
    start = steady_clock::now();
    conn.subscribeAll(bind(&Journaler::handleMessage, this, _1, _2));
    try {
        while (true) {
            while (queue.empty()) {
                cond.wait(lck);
            }
            while (!queue.empty()) {
                JournalEntry ent = queue.front();
                queue.pop();
                lck.unlock();
                journal.write(ent);
                lck.lock();
            }
            lck.unlock();
            journal.flush();
            lck.lock();
        }
    } catch (const std::exception &e) {
        conn.subscribeAll(function<void(const network::SubscriptionID &, const string &)>());
        throw;
    }
}
