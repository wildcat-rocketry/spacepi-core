#include <csignal>
#include <exception>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <linux/prctl.h>
#include <sys/mount.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <pwd.h>
#include <unistd.h>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/RemoteID.hpp>
#include <spacepi/spacepictl/Verb.hpp>

using namespace std;
using namespace spacepi::concurrent;
using namespace spacepi::log;
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

    struct statfs stat;
    if (statfs("/", &stat) < 0) {
        log(LogLevel::Warning) << "Unable to stat /: " << strerror(errno);
    } else if (mount(nullptr, "/", nullptr, MS_REMOUNT | (stat.f_flags & ~MS_RDONLY), nullptr) < 0) {
        log(LogLevel::Warning) << "Unable to remount / as read-write: " << strerror(errno);
    }

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
