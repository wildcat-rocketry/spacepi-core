#include <string>
#include <boost/filesystem.hpp>
#include <SpacePi.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/UniqueProcess.hpp>
#include <spacepi/target/Config.hpp>
#include <spacepi/target/DownloadFirmwareStep.hpp>
#include <spacepi/target/Firmware.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace spacepi::log;
using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::target;

void DownloadFirmwareStep::run(InstallationData &data) {
    Firmware fw;
    data.initData<Firmware>(fw);
    string repo = fw.getDownloadRepo();
    // For now, download using Git exe, not lib due to lib not supporting shallow clone
    if (is_directory(repo)) {
        UniqueProcess fetch(false, false, false, GIT_EXECUTABLE, { "-C", repo, "fetch", "--depth=1", "origin", "master" });
        fetch.wait();
        if (fetch.getExitCode() != 0) {
            log(LogLevel::Warning) << "Unable to check for new firmware.";
            return;
        }
        UniqueProcess checkout(false, false, false, GIT_EXECUTABLE, { "-C", repo, "checkout", "origin/master" });
        checkout.wait();
        if (checkout.getExitCode() != 0) {
            throw EXCEPTION(ResourceException("Error checking out Git firmware."));
        }
    } else {
        remove_all(repo + "~");
        UniqueProcess clone(false, false, false, GIT_EXECUTABLE, { "clone", "--depth=1", "https://github.com/raspberrypi/firmware.git", repo + "~" });
        clone.wait();
        if (clone.getExitCode() != 0) {
            throw EXCEPTION(ResourceException("Error downloading Git firmware."));
        }
        rename(repo + "~", repo);
    }
}
