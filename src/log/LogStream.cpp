#include <chrono>
#include <ostream>
#include <string>
#include <spacepi/log/Entry.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/log/LogManager.hpp>
#include <spacepi/log/LogStream.hpp>

using namespace std;
using namespace std::chrono;
using namespace spacepi::log;

LogStream::LogStream(const string &tag, const LogLevel &level, LogManager &manager) : ostream(&buffer), tag(tag), level(level), manager(manager), time(system_clock::now()) {
}

LogStream::LogStream(const LogStream &copy) : ostream(&buffer), tag(copy.tag), level(copy.level), manager(copy.manager), time(copy.time), buffer(copy.buffer.str()) {
}

LogStream::~LogStream() {
    string str = buffer.str();
    if (!str.empty()) {
        manager << Entry(tag, level, time, str);
    }
}
