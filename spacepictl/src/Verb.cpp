#include <cstdlib>
#include <exception>
#include <iomanip>
#include <ios>
#include <string>
#include <unordered_map>
#include <vector>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/Verb.hpp>

using namespace std;
using namespace spacepi::log;
using namespace spacepi::util;
using namespace spacepi::spacepictl;

Logger Verb::log("spacepictl");
string Verb::argv0 = "spacepictl";

int main(int argc, const char **argv) noexcept {
    return Verb::main(argc, argv);
}

int Verb::main(int argc, const char **argv) noexcept {
    try {
        --argc;
        argv0 = *argv++;
        string verbName;
        if (argc >= 1 && **argv != '-') {
            --argc;
            verbName = *argv++;
        }
        unordered_map<string, Verb *>::iterator it = getVerbs().find(verbName);
        if (it == getVerbs().end()) {
            printFullHelp(log(LogLevel::Error) << "Unknown verb: " << verbName << "\n");
            return EXIT_FAILURE;
        }
        vector<string> args;
        args.reserve(argc);
        for (int i = 0; i < argc; ++i) {
            args.emplace_back(argv[i]);
        }
        if (it->second->run(args)) {
            return EXIT_SUCCESS;
        }
    } catch (const std::exception &e) {
        log(LogLevel::Error) << "Error running spacepictl: " << e.what() << ": " << Exception::getPointer();
    }
    return EXIT_FAILURE;
}

ostream &Verb::printFullHelp(ostream &os) noexcept {
    os << "Usage: " << argv0 << " <verb> [<option> ...]\n"
          "\n"
          "Available verbs:\n" << left;
    int width = 0;
    for (unordered_map<string, Verb *>::const_iterator it = getVerbs().begin(); it != getVerbs().end(); ++it) {
        int w = it->first.size();
        if (w > width) {
            width = w;
        }
    }
    for (unordered_map<string, Verb *>::const_iterator it = getVerbs().begin(); it != getVerbs().end(); ++it) {
        os << "    " << setw(width) << it->first << setw(0) << "  " << it->second->desc << "\n";
    }
    os << "\n";
    for (unordered_map<string, Verb *>::const_iterator it = getVerbs().begin(); it != getVerbs().end(); ++it) {
        it->second->printHelp(os << it->second->desc << ": " << argv0 << " " << it->first) << "\n";
    }
    return os;
}

Verb::Verb(const string &name, const string &desc) noexcept : name(name), desc(desc) {
    getVerbs()[name] = this;
}

void Verb::printErrorHelp(const string &error) const noexcept {
    printHelp(log(LogLevel::Error) << error << "\nUsage: " << argv0 << " " << name);
}

unordered_map<string, Verb *> &Verb::getVerbs() noexcept {
    static unordered_map<string, Verb *> verbs;
    return verbs;
}
