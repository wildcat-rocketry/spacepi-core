#include <exception>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <spacepi/util/CommandConfigurable.hpp>
#include <spacepi/util/CommandInternals.hpp>

using namespace std;
using namespace spacepi::util::detail;

GenericCommandParser::operator bool() const noexcept {
    return valid;
}

pair<string, pair<vector<string>::const_iterator, string>> GenericCommandParser::parseNormal(const vector<string> &args, const vector<string>::const_iterator &start) noexcept {
    string::const_iterator sit = start->begin();
    if (sit == start->end() || *sit != '-') {
        return make_pair("", make_pair(start, ""));
    }
    ++sit;
    if (sit == start->end() || *sit != '-') {
        return make_pair("", make_pair(start, ""));
    }
    ++sit;
    string::const_iterator nit = name.begin();
    while (nit != name.end()) {
        if (sit == start->end() || *sit != *nit) {
            return make_pair("", make_pair(start, ""));
        }
        ++sit;
        ++nit;
    }
    if (sit == start->end()) {
        if (start + 1 == args.end()) {
            return make_pair("", make_pair(args.end(), "Option --" + name + " expects an argument"));
        }
        return make_pair(start[1], make_pair(start + 2, ""));
    } else if (*sit == '=' && start->length() > name.length() + 3) {
        return make_pair(start->substr(name.length() + 3), make_pair(start + 1, ""));
    } else {
        return make_pair("", make_pair(start, ""));
    }
}

GenericCommandParser::GenericCommandParser(const string &name, const string &desc, bool valid) : name(name), desc(desc), valid(valid) {
}

template <>
pair<vector<string>::const_iterator, string> CommandParser<string>::parse(const vector<string> &args, const vector<string>::const_iterator &start) noexcept {
    pair<string, pair<vector<string>::const_iterator, string>> res = parseNormal(args, start);
    if (!res.first.empty()) {
        var = res.first;
        valid = true;
    }
    return res.second;
}

template <>
string CommandParser<string>::example() const noexcept {
    return "--" + name + "=abc";
}

template <>
pair<vector<string>::const_iterator, string> CommandParser<vector<string>>::parse(const vector<string> &args, const vector<string>::const_iterator &start) noexcept {
    return parseVector(args, start, var);
}

template <>
string CommandParser<vector<string>>::example() const noexcept {
    return "--" + name + "=abc...";
}

template <>
pair<vector<string>::const_iterator, string> CommandParser<int>::parse(const vector<string> &args, const vector<string>::const_iterator &start) noexcept {
    pair<string, pair<vector<string>::const_iterator, string>> res = parseNormal(args, start);
    if (!res.first.empty()) {
        try {
            size_t idx = 0;
            var = stoi(res.first, &idx);
            if (idx != res.first.length()) {
                throw exception();
            }
            valid = true;
        } catch (const exception &) {
            return make_pair(res.second.first, "Invalid argument for --" + name);
        }
    }
    return res.second;
}

template <>
string CommandParser<int>::example() const noexcept {
    return "--" + name + "=123";
}

template <>
pair<vector<string>::const_iterator, string> CommandParser<vector<int>>::parse(const vector<string> &args, const vector<string>::const_iterator &start) noexcept {
    return parseVector(args, start, var);
}

template <>
string CommandParser<vector<int>>::example() const noexcept {
    return "--" + name + "=123...";
}

template <>
pair<vector<string>::const_iterator, string> CommandParser<double>::parse(const vector<string> &args, const vector<string>::const_iterator &start) noexcept {
    pair<string, pair<vector<string>::const_iterator, string>> res = parseNormal(args, start);
    if (!res.first.empty()) {
        try {
            size_t idx = 0;
            var = stod(res.first, &idx);
            if (idx != res.first.length()) {
                throw exception();
            }
            valid = true;
        } catch (const exception &) {
            return make_pair(res.second.first, "Invalid argument for --" + name);
        }
    }
    return res.second;
}

template <>
string CommandParser<double>::example() const noexcept {
    return "--" + name + "=12.3";
}

template <>
pair<vector<string>::const_iterator, string> CommandParser<vector<double>>::parse(const vector<string> &args, const vector<string>::const_iterator &start) noexcept {
    return parseVector(args, start, var);
}

template <>
string CommandParser<vector<double>>::example() const noexcept {
    return "--" + name + "=12.3...";
}

template <>
pair<vector<string>::const_iterator, string> CommandParser<bool>::parse(const vector<string> &args, const vector<string>::const_iterator &start) noexcept {
    if (*start == "--" + name) {
        var = true;
    } else if (*start == "--no-" + name) {
        var = false;
    } else {
        return make_pair(start, "");
    }
    valid = true;
    return make_pair(start + 1, "");
}

template <>
string CommandParser<bool>::example() const noexcept {
    return "--[no-]" + name;
}

CommandGroup::CommandGroup(const string &caption, CommandConfigurable &cmd) noexcept : caption(caption), cmd(cmd) {
}

CommandGroup &CommandGroup::operator =(const CommandGroup &copy) {
    if (&cmd != &copy.cmd) {
        throw invalid_argument("Cannot assign from a different CommandConfigurable");
    }
    caption = copy.caption;
    options = copy.options;
    return *this;
}

void CommandGroup::add(shared_ptr<GenericCommandParser> &&option) {
    options.push_back(move(option));
}
