#include <cstddef>
#include <exception>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
#include <spacepi/router/RouterEndpoint.hpp>
#include <spacepi/util/CommandInternals.hpp>

using namespace std;
using namespace spacepi::router;
using namespace spacepi::util::detail;

RouterEndpoint RouterEndpoint::defaultEndpoint = RouterEndpoint(25401);

RouterEndpoint::RouterEndpoint() noexcept : type(RouterEndpoint::Invalid) {
}

RouterEndpoint::RouterEndpoint(int port) noexcept : type(RouterEndpoint::TCP), port(port) {
}

RouterEndpoint::RouterEndpoint(const string &path) noexcept : type(RouterEndpoint::UNIX), path(path) {
}

bool RouterEndpoint::tryParse(const string &str, RouterEndpoint &endpoint) noexcept {
    try {
        if (str.substr(0, 4) == "tcp:") {
            size_t len = 0;
            endpoint = RouterEndpoint(stoi(str.substr(4), &len));
            return len + 4 == str.length() && endpoint.getPort() > 0 && endpoint.getPort() <= 65535;
        } else if (str.substr(0, 5) == "unix:") {
            endpoint = RouterEndpoint(str.substr(5));
            return true;
        } else {
            return false;
        }
    } catch (const exception &) {
        return false;
    }
}

string RouterEndpoint::toString() const noexcept {
    switch (type) {
        case TCP:
            return "tcp:" + to_string(port);
        case UNIX:
            return "unix:" + path;
        default:
            return "(invalid)";
    }
}

enum RouterEndpoint::Type RouterEndpoint::getType() const noexcept {
    return type;
}

int RouterEndpoint::getPort() const noexcept {
    return port;
}

const string &RouterEndpoint::getPath() const noexcept {
    return path;
}

ostream &operator <<(ostream &os, const RouterEndpoint &endpoint) {
    return os << endpoint.toString();
}

template <>
pair<vector<string>::const_iterator, string> CommandParser<RouterEndpoint>::parse(const vector<string> &args, const vector<string>::const_iterator &start) noexcept {
    pair<string, pair<vector<string>::const_iterator, string>> res = parseNormal(args, start);
    if (!res.first.empty()) {
        valid = RouterEndpoint::tryParse(res.first, var);
        if (!valid) {
            return make_pair(res.second.first, "Invalid endpoint format");
        }
    }
    return res.second;
}

template <>
string CommandParser<RouterEndpoint>::example() const noexcept {
    return "--" + name + "=" + RouterEndpoint::defaultEndpoint.toString();
}

template <>
pair<vector<string>::const_iterator, string> CommandParser<vector<RouterEndpoint>>::parse(const vector<string> &args, const vector<string>::const_iterator &start) noexcept {
    return parseVector(args, start, var);
}

template <>
string CommandParser<vector<RouterEndpoint>>::example() const noexcept {
    return "--" + name + "=" + RouterEndpoint::defaultEndpoint.toString() + "...";
}
