#include <exception>
#include <string>
#include <spacepi/log/LogFilter.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/util/Trie.hpp>

using namespace std;
using namespace spacepi::log;
using namespace spacepi::util;

LogFilter::LogFilter() noexcept : defaultLevel(&LogLevel::Info) {
}

bool LogFilter::operator ()(const string &name, const LogLevel &level) const noexcept {
    return level >= getLevel(name);
}

const LogLevel &LogFilter::getDefaultLevel() const noexcept {
    return *defaultLevel;
}

void LogFilter::setDefaultLevel(const LogLevel &level) noexcept {
    defaultLevel = &level;
}

const LogLevel &LogFilter::getLevel(const string &name) const noexcept {
    try {
        Trie<const LogLevel *>::ConstFindResult it = levels.find(name);
        if (it.hasData()) {
            return *it.data();
        }
    } catch (const exception &) {
    }
    return *defaultLevel;
}

void LogFilter::setLevel(const string &name, const LogLevel &level) noexcept {
    try {
        Trie<const LogLevel *>::FindResult it = levels.find(name);
        if (it.hasData()) {
            it.data() = &level;
        } else if (it.hasChild()) {
            levels.erase(name);
            levels.insert(name, &level);
        } else {
            levels.insert(name, &level);
        }
    } catch (const exception &) {
    }
}

void LogFilter::resetLevel(const string &name) noexcept {
    try {
        levels.erase(name);
    } catch (const exception &) {
    }
}
