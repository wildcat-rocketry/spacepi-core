#include <SpacePi.hpp>
#include <spacepi/liblinux/DefaultInstallationConfig.hpp>
#include <spacepi/liblinux/DefaultInstallationPlan.hpp>
#include <spacepi/liblinux/InstallationOptions.hpp>
#include <spacepi/liblinux/InstallationPlan.hpp>
#include <spacepi/liblinux/Partition.hpp>
#include <spacepi/liblinux/PartitionTable.hpp>
#include <spacepi/target/OSBuilder.hpp>

using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::target;

OSBuilder::OSBuilder(Command &cmd) : CommandConfigurable("OS Builder", cmd), opts(cmd) {
}

void OSBuilder::runCommand() {
    InstallationData d;
    d.initData<InstallationOptions>(opts);
    d.initData<InstallationConfig>(DefaultInstallationConfig());
    d.initData<PartitionTable>(PartitionTable());
    InstallationPlan p = DefaultInstallationPlan();
    p.install(d);
}
