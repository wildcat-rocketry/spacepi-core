#include <string>
#include <spacepi/package/Module.hpp>
#include <spacepi/package/Options.hpp>

using namespace std;
using namespace spacepi::package;

const string &Module::getName() const noexcept {
    return name;
}

void Module::setName(const string &name) noexcept {
    this->name = name;
}

const std::string &Module::getType() const noexcept {
    return type;
}

void Module::setType(const std::string &type) noexcept {
    this->type = type;
}

bool Module::hasAutomaticStart() const noexcept {
    return automaticStart;
}

void Module::setAutomaticStart(bool automaticStart) noexcept {
    this->automaticStart = automaticStart;
}

const Options &Module::getOptions() const noexcept {
    return options;
}

Options &Module::getOptions() noexcept {
    return options;
}
