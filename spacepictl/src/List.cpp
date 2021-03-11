#include <ostream>
#include <string>
#include <vector>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/Config.hpp>
#include <spacepi/spacepictl/List.hpp>
#include <spacepi/spacepictl/Verb.hpp>

using namespace std;
using namespace spacepi::log;
using namespace spacepi::package;
using namespace spacepi::spacepictl;

List List::instance;

List::List() noexcept : Verb("list", "List SpacePi services") {
}

bool List::run(const vector<string> &args) {
    if (!args.empty()) {
        printErrorHelp("list verb does not take any arguments.");
        return false;
    }

    PackageConfig cfg(SPACEPI_RUNNING_CONF);
    LogStream stream = log(LogLevel::Info);
    for(const auto& module_pair : cfg.getModules()) {
        stream << module_pair.second.getName() << "\n";
    }
    return true;
}

ostream &List::printHelp(ostream &os) const noexcept {
    return os << "\n";
}
