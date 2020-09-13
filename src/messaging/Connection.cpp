#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <google/protobuf/message.h>
#include <spacepi/messaging/Connection.hpp>
#include <spacepi/messaging/Subscription.hpp>

using namespace std;
using namespace boost::program_options;
using namespace google::protobuf;
using namespace spacepi::messaging;

Connection::Connection(vector<string> &args) : CommandConfigurable("Connection Options", args) {
}

Connection::~Connection() {
    // TODO
}

Connection &Connection::operator <<(const Message *message) {
    // TODO
    return *this;
}

void Connection::unsubscribe(uint32_t messageID) {
    // TODO
}

void Connection::waitForMessage(uint32_t messageID, void *ctx, void (*callback)(void *ctx, uint32_t messageID, uint32_t length, void *data)) {
    // TODO
}

void Connection::options(options_description &desc) const {
    // TODO
}

void Connection::configure(const parsed_options &opts) {
    // TODO
}
