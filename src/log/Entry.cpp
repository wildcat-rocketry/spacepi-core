#include <chrono>
#include <string>
#include <spacepi/log/Entry.hpp>
#include <spacepi/log/LogLevel.hpp>

using namespace std;
using namespace std::chrono;
using namespace spacepi::log;

Entry::Entry() noexcept : level(LogLevel::Error) {
}

Entry::Entry(const string &tag, const LogLevel &level, const system_clock::time_point &time, const std::string &message) noexcept : tag(tag), level(level), time(time), message(message) {
}

const string &Entry::getTag() const noexcept {
    return tag;
}

const LogLevel &Entry::getLevel() const noexcept {
    return level;
}

const system_clock::time_point &Entry::getTime() const noexcept {
    return time;
}

const string &Entry::getMessage() const noexcept {
    return message;
}
