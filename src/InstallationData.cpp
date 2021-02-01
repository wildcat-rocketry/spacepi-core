#include <spacepi/liblinux/InstallationData.hpp>

using namespace spacepi::liblinux::detail;

int GenericInstallationDataAccessor::getNextID() {
    static int prev = -1;
    return ++prev;
}
