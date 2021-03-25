#include <cstdint>
#include <memory>
#include <spacepi/messaging/Connection.hpp>
#include <spacepi/messaging/RawConnection.hpp>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

using namespace std;
using namespace spacepi::messaging;
using namespace spacepi::util;

Connection::Connection(Command &cmd) : conn(new RawConnection(cmd)) {
}

Publisher Connection::operator ()(uint64_t instanceID) const noexcept {
    return (*conn)(instanceID);
}

const shared_ptr<RawConnection> &Connection::raw() const noexcept {
    return conn;
}
