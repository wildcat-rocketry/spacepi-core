#include <ios>
#include <string>
#include <spacepi/example/ClassA.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

using namespace std;
using namespace spacepi::example;
using namespace spacepi::log;
using namespace spacepi::util;

ClassA::ClassA(Command &cmd) noexcept : CommandConfigurable("Class A Options", cmd) {
    fromCommand(boolVar, false, "abool", "A boolean option type");
    fromCommand(intVar, "aint", "An integer that is required");
    fromCommand(stringVar, "This is the default", "astring", "A string with a default value");
}

void ClassA::runCommand() {
    log(LogLevel::Info)
        << "From class A:\n"
        << "    Boolean variable: " << boolalpha << boolVar << "\n"
        << "    Integer variable: " << intVar << "\n"
        << "     String variable: " << stringVar;
}
