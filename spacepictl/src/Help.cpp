#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/Help.hpp>
#include <spacepi/spacepictl/Verb.hpp>

using namespace std;
using namespace spacepi::log;
using namespace spacepi::spacepictl;

Help Help::instance;

Help::Help() noexcept : Verb("help", "Shows this help message") {
}

bool Help::run(const vector<string> &args) {
    unordered_map<string, Verb *>::const_iterator it;
    switch (args.size()) {
        case 0:
            Verb::printFullHelp(log(LogLevel::Info) << "");
            return true;
        case 1:
            it = Verb::getVerbs().find(args.front());
            if (it == Verb::getVerbs().end()) {
                printErrorHelp("Unknown verb '" + args.front() + "'");
                return false;
            }
            it->second->printHelp(log(LogLevel::Info) << "Usage: " << Verb::argv0 << " " << it->first);
            return true;
        default:
            printErrorHelp("Too many arguments.");
            return false;
    }
}

ostream &Help::printHelp(ostream &os) const noexcept {
    return os << " [<verb>]\n"
                 "Options:\n"
                 "    verb  Only print help for a single verb\n";
}
