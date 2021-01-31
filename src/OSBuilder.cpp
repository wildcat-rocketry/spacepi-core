#include <fstream>
#include <iostream>
#include <SpacePi.hpp>
#include <spacepi/liblinux/Partition.hpp>
#include <spacepi/liblinux/PartitionTable.hpp>
#include <spacepi/liblinux/SharedMount.hpp>
#include <spacepi/liblinux/UniqueMount.hpp>
#include <spacepi/target/OSBuilder.hpp>
#include <spacepi/liblinux/UniqueTempDir.hpp>

using namespace std;
using namespace spacepi::util;
using namespace spacepi::target;
using namespace spacepi::liblinux;

OSBuilder::OSBuilder(Command &cmd) : CommandConfigurable("OS Builder", cmd) {

}

void OSBuilder::runCommand() {
    PartitionTable partTable;
    partTable.addPartition(Partition() .setUUID("thing0") .setMountPoint("/") .setFSType("ext4") .setOptions("defaults,sync,noatime,ro") .setSize("256M") .setType("0C"));
    partTable.addPartition(Partition() .setUUID("thing1") .setMountPoint("/boot") .setFSType("vfat") .setOptions("defaults,sync,noatime,ro") .setSize("768M") .setType("0F"));
    partTable.printFstab(cout);
    cout << std::endl;
    partTable.setLabel("dos");
    partTable.setUnit("sectors");
    partTable.printSfdisk(cout);

    cout << "Starting Mounting Test....." << std::endl;
    SharedMount sharedMount("/home","/tmp/home","bind,defaults,ro","none");
    cout << "Starting TempDir Test....." << std::endl;
    UniqueTempDir uniqueTempDir("test");
    cout << uniqueTempDir.getPath() << endl;
    ofstream file1(uniqueTempDir.getPath() + "/test.txt");
    file1 << "Woo" << endl;
    file1.close();
    sleep(60);
}
