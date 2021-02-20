#include <memory>
#include <vector>
#include <spacepi/liblinux/InstallationData.hpp>

using namespace std;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::detail;

int GenericInstallationDataAccessor::getNextID() {
    static int prev = -1;
    return ++prev;
}

InstallationData::~InstallationData() noexcept(false) {
    for (vector<shared_ptr<GenericInstallationDataAccessor>>::iterator it = data.begin(); it != data.end(); ++it) {
        if (*it && it->unique()) {
            (*it)->deleteData();
        }
    }
}
