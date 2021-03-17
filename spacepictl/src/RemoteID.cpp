#include <csignal>
#include <exception>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <linux/prctl.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/RemoteID.hpp>
#include <spacepi/spacepictl/Verb.hpp>

using namespace std;
using namespace spacepi::concurrent;
using namespace spacepi::spacepictl;

RemoteID RemoteID::instance;

RemoteID::RemoteID() noexcept : Verb("remote-id", "Prints out the current username, then blocks forever", true) {
}

bool RemoteID::run(const vector<string> &args) {
    if (!args.empty()) {
        printErrorHelp("remote-id verb does not take any arguments.");
        return false;
    }

    prctl(PR_SET_PDEATHSIG, SIGHUP);

    ostringstream ss;
    ss << "\n"
          "spacepi-targetlib-linux:USER=" << getpwuid(getuid())->pw_name << "\n";
    cerr << ss.str();

    try {
        Sleep::forever();
    } catch (const exception &) {
    }
    return true;
}

ostream &RemoteID::printHelp(ostream &os) const noexcept {
    return os << "\n";
}
