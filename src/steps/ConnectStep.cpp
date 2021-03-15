#include <exception>
#include <string>
#include <vector>
#include <sys/types.h>
#include <grp.h>
#include <unistd.h>
#include <boost/filesystem.hpp>
#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/ConnectStep.hpp>
#include <spacepi/liblinux/InstallationConfig.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/SharedMount.hpp>
#include <spacepi/liblinux/SharedTempDir.hpp>
#include <spacepi/liblinux/State.hpp>
#include <spacepi/liblinux/SystemCaller.hpp>
#include <spacepi/liblinux/UniqueChroot.hpp>
#include <spacepi/liblinux/UniqueEID.hpp>
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
    vector<SharedMount> nfs;
    nfs.reserve(config.nfsDirs.size());
    try {
        for (vector<string>::const_iterator it = config.nfsDirs.begin(); it != config.nfsDirs.end(); ++it) {
            nfs.emplace_back("127.0.0.1:" + *it, (root / *it).native(), "port=" + to_string(state->sshport()) + ",vers=4.0,addr=127.0.0.1", "nfs");
        }
    } catch (const exception &e) {
        log(LogLevel::Warning) << "Unable to mount NFS share.  Do you have 'nfs-common' installed?";
        throw;
    }
    UniqueChroot chroot(root.native());
    vector<gid_t> grp;
    grp.reserve(state->sshgrp_size());
    for (int i = 0; i < state->sshgrp_size(); ++i) {
        grp.push_back(state->sshgrp(i));
    }
    handle(setgroups(grp.size(), grp.data()))
        << "Unable to set groups: " << SyscallErrorString;
    UniqueEID eid(state->sshuid(), state->sshgid());
    UniqueProcess proc("/bin/sh", {});
    proc.wait();
}
