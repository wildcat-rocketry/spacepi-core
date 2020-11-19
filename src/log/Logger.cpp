#include <string>
#include <utility>
#include <spacepi/log/Logger.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/log/LogManager.hpp>
#include <spacepi/log/LogStream.hpp>

using namespace std;
using namespace spacepi::log;

Logger::Logger(const string &tag, LogManager &manager) noexcept : tag(tag), manager(manager) {
}

LogStream Logger::operator ()(const LogLevel &level) const noexcept {
    return LogStream(tag, level, manager);
}
