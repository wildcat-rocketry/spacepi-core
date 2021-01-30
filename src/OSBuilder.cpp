#include <fstream>
#include <iostream>
#include <SpacePi.hpp>
#include <spacepi/liblinux/Partition.hpp>
#include <spacepi/liblinux/PartitionTable.hpp>
#include <spacepi/liblinux/SharedMount.hpp>
#include <spacepi/liblinux/UniqueMount.hpp>
#include <spacepi/target/OSBuilder.hpp>

using namespace std;
using namespace spacepi::util;
using namespace spacepi::target;
using namespace spacepi::liblinux;

OSBuilder::OSBuilder(Command &cmd) : CommandConfigurable("OS Builder", cmd) {
    fromCommand(configFile, "config-file", "XML file path to configure the OS from");
    fromCommand(outFile, "out", "Output image file path");
    fromCommand(dataDir, "data-dir", "The source directory of the OS target to get data files from");
}

void OSBuilder::runCommand() {
    cout << "This is doing something." << endl;
    cerr << "This could be an error message." << endl;
    ofstream file(outFile);
    file << "Config File: " << configFile << endl
         << "Output File: " << outFile << endl
         << "   Data Dir: " << dataDir << endl;
    cout << "File written to " << outFile << endl;

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
    sleep(60);
}
