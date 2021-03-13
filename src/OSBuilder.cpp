#include <string>
#include <vector>
#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/EnsureRootStep.hpp>
#include <spacepi/liblinux/steps/InstallSystemFilesStep.hpp>
#include <spacepi/liblinux/DefaultInstallationConfig.hpp>
#include <spacepi/liblinux/InstallationOptions.hpp>
#include <spacepi/liblinux/InstallationPlan.hpp>
#include <spacepi/liblinux/Installer.hpp>
#include <spacepi/liblinux/Partition.hpp>
#include <spacepi/liblinux/PartitionTable.hpp>
#include <spacepi/target/Config.hpp>
#include <spacepi/target/DownloadFirmwareStep.hpp>
#include <spacepi/target/InstallFirmwareStep.hpp>
#include <spacepi/target/OSBuilder.hpp>

using namespace std;
using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;
using namespace spacepi::target;

OSBuilder::OSBuilder(Command &cmd) : CommandConfigurable("OS Builder", cmd), opts(cmd) {
}

void OSBuilder::runCommand() {
    InstallationData d;
    InstallationConfig cfg = DefaultInstallationConfig();
    cfg.dpkgArch = "armel";
    cfg.emulationBin = { QEMU_ARM_STATIC_EXECUTABLE };
    d.initData<InstallationOptions>(opts);
    d.initData<InstallationConfig>(cfg);
    d.initData<PartitionTable>(PartitionTable()
        .setLabel("dos").setSize("4G")
        .addPartition(Partition()
            .setSize("256M")
            .setType("0C")
            .setMountPoint("/boot")
            .setFSType("vfat")
            .setFormatOptions(vector<string> { "-F", "32" })
            .setOptions("defaults,sync,noatime,ro")
            .setPass(2))
        .addPartition(Partition()
            .setSize("2G")
            .setType("83")
            .setMountPoint("/")
            .setFSType("ext4")
            .setOptions("defaults,sync,noatime,ro")
            .setPass(1))
        .addPartition(Partition()
            .setType("83")
            .setMountPoint("/var")
            .setFSType("ext4")
            .setOptions("defaults,sync,noatime")
            .setDump(1)
            .setPass(2))
        .addPartition(Partition()
            .setSource("none")
            .setMountPoint("/tmp")
            .setFSType("tmpfs")
            .setOptions("defaults,noatime"))
        .addPartition(Partition()
            .setSource("none")
            .setMountPoint("/run")
            .setFSType("tmpfs")
            .setOptions("defaults,noatime")));
    Installer inst;
    InstallationPlan &p = inst.getInstallPlan();
    p.insertStepAfter<EnsureRootStep, DownloadFirmwareStep>();
    p.insertStepBefore<InstallSystemFilesStep, InstallFirmwareStep>();
    inst.install(d);
}
