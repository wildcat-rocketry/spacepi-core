#include <string>
#include <vector>
#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/EnsureRootStep.hpp>
#include <spacepi/liblinux/steps/InstallSystemFilesStep.hpp>
#include <spacepi/liblinux/DefaultInstallationConfig.hpp>
#include <spacepi/liblinux/DefaultInstallationPlan.hpp>
#include <spacepi/liblinux/InstallationOptions.hpp>
#include <spacepi/liblinux/InstallationPlan.hpp>
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
        .addPartition(Partition().setSize("256M").setType("0C").setFSType("vfat").setFormatOptions(vector<string> { "-F", "32" }).setMountPoint("/boot").setOptions("defaults"))
        .addPartition(Partition().setSize("2G").setType("83").setFSType("ext4").setMountPoint("/var").setOptions("defaults"))
        .addPartition(Partition().setType("83").setFSType("ext4").setMountPoint("/").setOptions("defaults,ro")));
    InstallationPlan p = DefaultInstallationPlan();
    p.insertStepAfter<EnsureRootStep, DownloadFirmwareStep>();
    p.insertStepBefore<InstallSystemFilesStep, InstallFirmwareStep>();
    p.install(d);
}
