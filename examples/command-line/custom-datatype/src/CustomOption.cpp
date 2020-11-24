#include <spacepi/example/CustomOption.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

using namespace spacepi::example;
using namespace spacepi::log;
using namespace spacepi::util;

CustomOption::CustomOption(Command &cmd) noexcept : CommandConfigurable("Custom Options", cmd) {
    fromCommand(customVar, "custom", "A variable with a custom data type");
}

void CustomOption::runCommand() {
    log(LogLevel::Info) << "Custom variable is: " << customVar;
}
