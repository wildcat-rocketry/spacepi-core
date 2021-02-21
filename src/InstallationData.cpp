#include <algorithm>
#include <memory>
#include <utility>
#include <vector>
#include <spacepi/liblinux/InstallationData.hpp>

using namespace std;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::detail;

int GenericInstallationDataAccessor::getNextID() {
    static int prev = -1;
    return ++prev;
}

InstallationData::InstallationData() noexcept : numObjs(0) {
}

InstallationData::~InstallationData() noexcept(false) {
    sort(data.begin(), data.end(), sortDtorOrder);
    for (vector<pair<int, shared_ptr<GenericInstallationDataAccessor>>>::iterator it = data.begin(); it != data.end(); ++it) {
        if (it->second && it->second.unique()) {
            it->second->deleteData();
        }
    }
}

bool InstallationData::sortDtorOrder(const pair<int, shared_ptr<GenericInstallationDataAccessor>> &a, const pair<int, shared_ptr<GenericInstallationDataAccessor>> &b) {
    return a.first > b.first;
}
