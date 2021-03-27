#include <cstdint>
#include <string>
#include <vector>
#include <spacepi/messaging/JournalEntry.pb.h>
#include <spacepi/resource/Filesystem.hpp>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>
#include <spacepi/journaler/Journal.hpp>

using namespace std;
using namespace spacepi::messaging::detail;
using namespace spacepi::resource;
using namespace spacepi::util;
using namespace spacepi::journaler;

Journal::Journal(Command &cmd) noexcept : CommandConfigurable("Journal Options", cmd) {
    fromCommand(fs, "file", "The file to use as the journal");
}

void Journal::open(bool write) {
    stream = fs->open("", write ? Filesystem::Log : Filesystem::Read);
}

bool Journal::read(JournalEntry &ent) const {
    ent.Clear();
    uint8_t data[3];
    stream->read((char *) data, 3);
    if (stream->fail()) {
        return false;
    }
    uint32_t len = (((uint32_t) data[0]) << 16)
                 | (((uint32_t) data[1]) <<  8)
                 | (((uint32_t) data[2]) <<  0);
    vector<char> buf(len);
    stream->read(buf.data(), len);
    ent.ParseFromString(string(buf.data(), len));
    return true;
}

void Journal::write(const JournalEntry &ent) const {
    string msg = ent.SerializeAsString();
    uint8_t data[3];
    data[0] = (msg.size() & 0xFF0000) >> 16;
    data[1] = (msg.size() & 0x00FF00) >>  8;
    data[2] = (msg.size() & 0x0000FF) >>  0;
    stream->write((char *) data, 3);
    stream->write(msg.data(), msg.size());
}

void Journal::flush() const {
    stream->flush();
}

void Journal::runCommand() {
}
