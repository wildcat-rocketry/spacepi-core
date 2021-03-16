#include <exception>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <unistd.h>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/RemoteID.hpp>
#include <spacepi/spacepictl/Verb.hpp>

using namespace std;
using namespace std::chrono;
using namespace spacepi::concurrent;
using namespace spacepi::spacepictl;

RemoteID RemoteID::instance;

RemoteID::RemoteID() noexcept : Verb("remote-id", "Prints out the current UID and GID, then blocks forever", true) {
}

bool RemoteID::run(const vector<string> &args) {
    if (!args.empty()) {
        printErrorHelp("remote-id verb does not take any arguments.");
        return false;
    }

    ostringstream ss;
    ss << "\n"
          "spacepi-targetlib-linux:UID=" << getuid() << "\n"
          "spacepi-targetlib-linux:GID=" << getgid() << "\n"
          "spacepi-targetlib-linux:GRP=";
    int count = getgroups(0, nullptr);
    vector<gid_t> groups;
    groups.resize(count);
    getgroups(count, groups.data());
    for (vector<gid_t>::const_iterator it = groups.begin(); it != groups.end(); ++it) {
        if (it != groups.begin()) {
            ss << ",";
        }
        ss << *it;
    }
    ss << "\n";
    cerr << ss.str();

    try {
        while (true) {
            Sleep::duration(nanoseconds::max());
        }
    } catch (const exception &) {
    }
    return true;
}

ostream &RemoteID::printHelp(ostream &os) const noexcept {
    return os << "\n";
}
