#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <unistd.h>
#include <linux/limits.h>
#include <SpacePi.hpp>
#include <spacepi/target/extension/DigitalIOChip.hpp>
#include <spacepi/target/extension/DigitalIOFactory.hpp>

using namespace std;
using namespace spacepi;
using namespace spacepi::resource;
using namespace spacepi::util;
using namespace spacepi::target::extension;

DigitalIOFactory DigitalIOFactory::instance;

DigitalIOFactory::DigitalIOFactory() : ResourceFactory<resource::DigitalIO>("gpio") {
    char path[PATH_MAX];
    int res = readlink("/proc/self/exe", path, PATH_MAX);
    if (res < 0) {
        throw EXCEPTION(ResourceException("Unable to identify process name."));
    }
    string exe(path, res);
    size_t slash = exe.find_last_of('/');
    if (slash == string::npos) {
        consumerName = exe;
    } else {
        consumerName = exe.substr(slash + 1);
    }
}

const string &DigitalIOFactory::getConsumerName() const noexcept {
    return consumerName;
}

void DigitalIOFactory::destroyChip(const string &name) {
    chips.erase(name);
}

shared_ptr<resource::DigitalIO> DigitalIOFactory::create(const string &name) {
    size_t colon = name.find_first_of(':');
    if (colon == string::npos) {
        throw EXCEPTION(ResourceException("Invalid GPIO resource name."));
    }
    string chipName = name.substr(0, colon);
    string lineName = name.substr(colon);
    unordered_map<string, weak_ptr<DigitalIOChip>>::iterator it = chips.find(name);
    shared_ptr<DigitalIOChip> chip;
    if (it == chips.end()) {
        chip.reset(new DigitalIOChip(*this, move(chipName)));
        chips.emplace_hint(it, piecewise_construct, make_tuple(chipName), make_tuple(chip));
    } else {
        chip = it->second.lock();
        if (!chip) {
            chip.reset(new DigitalIOChip(*this, move(chipName)));
            it->second = weak_ptr<DigitalIOChip>(chip);
        }
    }
    return chip->addLine(lineName);
}
