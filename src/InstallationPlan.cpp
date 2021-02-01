#include <list>
#include <SpacePi.hpp>
#include <spacepi/liblinux/InstallationPlan.hpp>

using namespace std;
using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::detail;

int GenericTypedInstaller::getNextID() {
    static int prev = -1;
    return ++prev;
}

list<unique_ptr<GenericTypedInstaller>>::const_iterator InstallationPlan::find(int id) const {
    for (list<unique_ptr<GenericTypedInstaller>>::const_iterator it = steps.begin(); it != steps.end(); ++it) {
        if ((*it)->getID() == id) {
            return it;
        }
    }
    return steps.end();
}

list<unique_ptr<GenericTypedInstaller>>::const_iterator InstallationPlan::findOrThrow(int id) const {
    list<unique_ptr<GenericTypedInstaller>>::const_iterator it = find(id);
    if (it == steps.end()) {
        throw EXCEPTION(ArgumentException("The specified type was not found in the InstallationPlan."));
    }
    return it;
}

void InstallationPlan::throwIfContains(int id) const {
    if (find(id) != steps.end()) {
        throw EXCEPTION(ArgumentException("The specified type was already found in the InstallationPlan."));
    }
}
