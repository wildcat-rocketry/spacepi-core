#include <exception>
#include <memory>
#include <string>
#include <boost/filesystem.hpp>
#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/ConnectStep.hpp>
#include <spacepi/liblinux/InstallationConfig.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/SharedTempDir.hpp>
#include <spacepi/liblinux/State.hpp>
#include <spacepi/liblinux/UniqueChroot.hpp>
#include <spacepi/liblinux/UniqueEID.hpp>
#include <spacepi/liblinux/UniqueMount.hpp>
#include <spacepi/liblinux/UniqueProcess.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace spacepi::log;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::detail;
using namespace spacepi::liblinux::steps;

void ConnectStep::run(InstallationData &data) {
    path root = data.getData<SharedTempDir>().getPath();
    const State &state = data.getData<State>();
    const InstallationConfig &config = data.getData<InstallationConfig>();
    unique_ptr<UniqueMount> nfs;
    try {
        nfs.reset(new UniqueMount("127.0.0.1:" + config.nfsDir, (root / config.nfsDir).native(), "port=" + to_string(state->sshport()) + ",vers=4.0,addr=127.0.0.1", "nfs"));
    } catch (const exception &e) {
        log(LogLevel::Warning) << "Unable to mount NFS share.  Do you have 'nfs-common' installed?";
        throw;
    }
    UniqueChroot chroot(root.native());
    UniqueEID eid(state->sshuid(), state->sshgid());
    UniqueProcess proc("/bin/sh", {});
    proc.wait();
}
