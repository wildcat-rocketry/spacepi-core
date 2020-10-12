#include <chrono>
#include <string>
#include <spacepi/log/Entry.hpp>
#include <spacepi/log/LogLevel.hpp>

using namespace std;
using namespace std::chrono;
using namespace spacepi::log;

Entry::Entry() : level(LogLevel::Error) {
}

Entry::Entry(const string &tag, const LogLevel &level, const system_clock::time_point &time, const std::string &message) : tag(tag), level(level), time(time), message(message) {
}

const string &Entry::getTag() const {
    return tag;
}

const LogLevel &Entry::getLevel() const {
    return level;
}

const system_clock::time_point &Entry::getTime() const {
    return time;
}

const string &Entry::getMessage() const {
    return message;
}
