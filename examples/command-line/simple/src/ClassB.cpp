#include <ios>
#include <string>
#include <vector>
#include <spacepi/example/ClassB.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

using namespace std;
using namespace spacepi::example;
using namespace spacepi::log;
using namespace spacepi::util;

ClassB::ClassB(Command &cmd) noexcept : CommandConfigurable("Class B Options", cmd) {
    fromCommand(boolVar, false, "bbool", "Another boolean option");
    fromCommand(stringList, vector<string>(), "bstrings", "A list of strings");
}

void ClassB::runCommand() {
    log(LogLevel::Info)
        << "From class B:\n"
        << "     Boolean variable: " << boolalpha << boolVar << "\n"
        << "    Number of strings: " << stringList.size() << "\n"
        << "        String values:\n";
    for (vector<string>::const_iterator it = stringList.begin(); it != stringList.end(); ++it) {
        log(LogLevel::Info) << "                Value: " << *it;
    }
}
