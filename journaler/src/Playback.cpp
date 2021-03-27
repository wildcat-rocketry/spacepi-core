#include <chrono>
#include <cstdint>
#include <string>
#include <spacepi/concurrent/Sleep.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/messaging/HelloMessage.pb.h>
#include <spacepi/messaging/JournalEntry.pb.h>
#include <spacepi/messaging/MessageID.pb.h>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>
#include <spacepi/journaler/FullConnection.hpp>
#include <spacepi/journaler/Journal.hpp>
#include <spacepi/journaler/Playback.hpp>

using namespace std;
using namespace std::chrono;
using namespace spacepi::concurrent;
using namespace spacepi::messaging;
using namespace spacepi::messaging::detail;
using namespace spacepi::util;
using namespace spacepi::journaler;

Playback::Playback(Command &cmd, Journal &journal, FullConnection &conn) noexcept : CommandConfigurable("Playback Options", cmd), journal(journal), conn(conn), enable(false) {
    fromCommand(enable, false, "playback", "Play back a journal onto the messaging bus");
}

void Playback::runCommand() {
    if (!enable) {
        return;
    }
    static const uint64_t helloID = HelloMessage::GetDescriptor()->options().GetExtension(MessageID);
    JournalEntry ent;
    journal.open(false);
    steady_clock::time_point start = steady_clock::now();
    while (journal.read(ent)) {
        if (ent.messageid() == helloID) {
            continue;
        }
        network::SubscriptionID id(ent.messageid(), ent.instanceid());
        const string &msg = ent.payload();
        Sleep::until(start + nanoseconds(ent.timestamp()));
        conn.send(id, msg);
    }
}
