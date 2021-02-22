#include <string>
#include <boost/filesystem.hpp>
#include <spacepi/target/Firmware.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace spacepi::target;

Firmware::Firmware() : downloadRepo((read_symlink("/proc/self/exe").parent_path() / ".firmware").native()) {
}

const string &Firmware::getDownloadRepo() const noexcept {
    return downloadRepo;
}
