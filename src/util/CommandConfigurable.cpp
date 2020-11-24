#include <string>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>
#include <spacepi/util/CommandInternals.hpp>

using namespace std;
using namespace spacepi::util;

CommandConfigurable::CommandConfigurable(const string &caption, Command &cmd) noexcept : group(*cmd.groups.emplace(cmd.groups.end(), caption, *this)) {
}
