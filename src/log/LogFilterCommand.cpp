#include <string>
#include <utility>
#include <vector>
#include <spacepi/log/LogFilter.hpp>
#include <spacepi/log/LogFilterCommand.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/log/LogManager.hpp>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>
#include <spacepi/util/CommandInternals.hpp>

using namespace std;
using namespace spacepi::log;
using namespace spacepi::util;
using namespace spacepi::util::detail;

LogFilterCommand::LogFilterCommand(Command &cmd) noexcept : CommandConfigurable("Logging Options", cmd) {
    fromCommand(defaultLevel, &LogLevel::Info, "logDefault", "The default minimum level to log on");
    fromCommand(debugLevel, vector<string>(), "logDebug", "A list of loggers to log a minimum of debug messages on");
    fromCommand(infoLevel, vector<string>(), "logInfo", "A list of loggers to log a minimum of info messages on");
    fromCommand(warningLevel, vector<string>(), "logWarning", "A list of loggers to log a minimum of warning messages on");
    fromCommand(errorLevel, vector<string>(), "logError", "A list of loggers to log a minimum of error messages on");
}

void LogFilterCommand::runCommand() {
    LogFilter &filter = LogManager::instance.getFilter();
    filter.setDefaultLevel(*defaultLevel);
    for (vector<string>::const_iterator it = debugLevel.begin(); it != debugLevel.end(); ++it) {
        filter.setLevel(*it, LogLevel::Debug);
    }
    for (vector<string>::const_iterator it = infoLevel.begin(); it != infoLevel.end(); ++it) {
        filter.setLevel(*it, LogLevel::Info);
    }
    for (vector<string>::const_iterator it = warningLevel.begin(); it != warningLevel.end(); ++it) {
        filter.setLevel(*it, LogLevel::Warning);
    }
    for (vector<string>::const_iterator it = errorLevel.begin(); it != errorLevel.end(); ++it) {
        filter.setLevel(*it, LogLevel::Error);
    }
}

template <>
pair<vector<string>::const_iterator, string> CommandParser<const LogLevel *>::parse(const vector<string> &args, const vector<string>::const_iterator &start) noexcept {
    pair<string, pair<vector<string>::const_iterator, string>> res = parseNormal(args, start);
    if (!res.first.empty()) {
        if (res.first == "debug" || res.first == "DEBUG") {
            var = &LogLevel::Debug;
        } else if (res.first == "info" || res.first == "INFO") {
            var = &LogLevel::Info;
        } else if (res.first == "warning" || res.first == "WARNING" || res.first == "warn" || res.first == "WARNING") {
            var = &LogLevel::Warning;
        } else if (res.first == "error" || res.first == "ERROR") {
            var = &LogLevel::Error;
        } else {
            return make_pair(res.second.first, "Unknown log level");
        }
        valid = true;
    }
    return res.second;
}

template <>
string CommandParser<const LogLevel *>::example() const noexcept {
    return "--" + name + "=INFO";
}
