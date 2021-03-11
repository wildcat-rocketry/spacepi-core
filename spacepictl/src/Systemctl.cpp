#include <array>
#include <cstring>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/util/System.hpp>
#include <spacepi/spacepictl/Config.hpp>
#include <spacepi/spacepictl/Systemctl.hpp>
#include <spacepi/spacepictl/Verb.hpp>

using namespace std;
using namespace spacepi::log;
using namespace spacepi::package;
using namespace spacepi::spacepictl;
using namespace spacepi::spacepictl::util;

Systemctl Systemctl::disable("disable");
Systemctl Systemctl::enable("enable");
Systemctl Systemctl::reenable("reenable");
Systemctl Systemctl::restart("restart");
Systemctl Systemctl::start("start");
Systemctl Systemctl::status("status");
Systemctl Systemctl::stop("stop");

Systemctl::Systemctl(const string &verb) noexcept : Verb(verb, "Wrapper around systemctl " + verb), verb(verb.size() + 1) {
    strcpy(this->verb.data(), verb.data());
}

bool Systemctl::run(const vector<string> &args) {
    if (args.size() != 1) {
        printErrorHelp("Incorrect number of arguments.");
        return false;
    }

    PackageConfig cfg(SPACEPI_RUNNING_CONF);
    unordered_map<string, Module>::iterator it = cfg.getModules().find(args.front());
    if (it == cfg.getModules().end()) {
        printErrorHelp("Module \"" + args.front() + "\" not found");
        return false;
    }

    array<char, sizeof(SYSTEMCTL_EXECUTABLE)> systemctl;
    strcpy(systemctl.data(), SYSTEMCTL_EXECUTABLE);
    vector<char *> argv;
    argv.reserve(3);
    argv.push_back(systemctl.data());
    argv.push_back(verb.data());
    string nameStr = System::moduleServiceName(it->second);
    vector<char> name(nameStr.size() + 1);
    strcpy(name.data(), nameStr.data());
    argv.push_back(name.data());
    argv.push_back(nullptr);

    execv(systemctl.data(), argv.data());

    LogStream ss = log(LogLevel::Error);
    ss << "Error executing";
    for(auto &arg : args) ss << " " << arg;

    return false;
}

ostream &Systemctl::printHelp(ostream &os) const noexcept {
    return os << " <name>\n"
                 "Options:\n"
                 "    name  The name of the service unit to pass to systemctl\n";
}
