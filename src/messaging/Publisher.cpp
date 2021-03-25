#include <cstddef>
#include <cstdint>
#include <ios>
#include <memory>
#include <string>
#include <google/protobuf/message.h>
#include <spacepi/concurrent/UniqueConditionVariableLock.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/log/LogStream.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/messaging/MessageID.pb.h>
#include <spacepi/messaging/Publisher.hpp>
#include <spacepi/messaging/RawConnection.hpp>

using namespace std;
using namespace google::protobuf;
using namespace spacepi;
using namespace spacepi::concurrent;
using namespace spacepi::log;
using namespace spacepi::messaging;
using namespace spacepi::messaging::network;

const Publisher &Publisher::operator <<(const Message &message) const {
    uint64_t messageID = message.GetDescriptor()->options().GetExtension(MessageID);
    if (conn->debugPublishes()) {
        LogStream str = log(log::LogLevel::Debug);
        str << "Publish " << message.GetDescriptor()->name() << "(" << showbase << hex << messageID << dec << "/" << instanceID << "):";
        string msg = message.DebugString();
        for (size_t start = 0, end = 0; start < msg.size() && end != string::npos; start = end + 1) {
            end = msg.find_first_of('\n', start);
            str << "\n    " << msg.substr(start, end - start);
        }
    }
    conn->send(SubscriptionID(messageID, instanceID), message.SerializeAsString());
    return *this;
}

Publisher::Publisher(const shared_ptr<RawConnection> &conn, uint64_t instanceID) noexcept : conn(conn), instanceID(instanceID) {
}
