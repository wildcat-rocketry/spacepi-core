#include <string>
#include <utility>
#include <spacepi/log/Logger.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/log/LogManager.hpp>
#include <spacepi/log/LogStream.hpp>

using namespace std;
using namespace spacepi::log;

Logger::Logger(const string &tag, LogManager &manager) : tag(tag), manager(manager) {
}

LogStream Logger::operator ()(const LogLevel &level) const {
    return LogStream(tag, level, manager);
}
