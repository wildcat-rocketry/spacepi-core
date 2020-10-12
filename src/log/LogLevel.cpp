#include <ostream>
#include <string>
#include <spacepi/log/LogLevel.hpp>

using namespace std;
using namespace spacepi::log;

const LogLevel LogLevel::Debug  (0, "DEBUG");
const LogLevel LogLevel::Info   (1, "INFO ");
const LogLevel LogLevel::Warning(2, "WARN ");
const LogLevel LogLevel::Error  (3, "ERROR");

LogLevel::LogLevel(int id, const string &tag) : id(id), tag(tag) {
}

bool LogLevel::operator ==(const LogLevel &other) const {
    return id == other.id;
}

bool LogLevel::operator !=(const LogLevel &other) const {
    return id != other.id;
}

bool LogLevel::operator <(const LogLevel &other) const {
    return id < other.id;
}

bool LogLevel::operator <=(const LogLevel &other) const {
    return id <= other.id;
}

bool LogLevel::operator >(const LogLevel &other) const {
    return id > other.id;
}

bool LogLevel::operator >=(const LogLevel &other) const {
    return id >= other.id;
}

const string &LogLevel::getTag() const {
    return tag;
}

ostream &operator <<(ostream &os, const LogLevel &level) {
    return os << level.getTag();
}
