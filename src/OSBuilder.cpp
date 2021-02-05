#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <SpacePi.hpp>
#include <spacepi/liblinux/Partition.hpp>
#include <spacepi/liblinux/PartitionTable.hpp>
#include <spacepi/liblinux/SharedMount.hpp>
#include <spacepi/liblinux/UniqueMount.hpp>
#include <spacepi/target/OSBuilder.hpp>
#include <spacepi/liblinux/UniqueTempDir.hpp>
#include <spacepi/liblinux/UniqueProcess.hpp>

using namespace std;
using namespace std::chrono;
using namespace spacepi::concurrent;
using namespace spacepi::log;
using namespace spacepi::util;
using namespace spacepi::target;
using namespace spacepi::liblinux;

OSBuilder::OSBuilder(Command &cmd) : CommandConfigurable("OS Builder", cmd) {

}

void OSBuilder::runCommand() {
    Logger log("spacepi:osbuilder");

    PartitionTable partTable;
    partTable.addPartition(Partition() .setUUID("thing0") .setMountPoint("/") .setFSType("ext4") .setOptions("defaults,sync,noatime,ro") .setSize("256M") .setType("0C"));
    partTable.addPartition(Partition() .setUUID("thing1") .setMountPoint("/boot") .setFSType("vfat") .setOptions("defaults,sync,noatime,ro") .setSize("768M") .setType("0F"));
    {
        LogStream stream = log(LogLevel::Info);
        partTable.printFstab(stream);
    }
    partTable.setLabel("dos");
    partTable.setUnit("sectors");
    {
        LogStream stream = log(LogLevel::Info);
        partTable.printSfdisk(stream);
    }

    log(LogLevel::Info) << "Starting Mounting Test.....";
    SharedMount sharedMount("/home","/tmp/home","bind,defaults,ro","none");
    log(LogLevel::Info) << "Starting TempDir Test.....";
    UniqueTempDir uniqueTempDir("test");
    log(LogLevel::Info) << uniqueTempDir.getPath();
    ofstream file1(uniqueTempDir.getPath() + "/test.txt");
    file1 << "Woo" << endl;
    file1.close();

    // Temporary workaround to keep network thread alive
    UniqueProcess p0(false, false, false, "/bin/sleep", "infinity");

    UniqueProcess p1(true, true, false, "/bin/cat", "--");
    log(LogLevel::Info) << "Writing Hello, world!";
    p1.input() << "Hello, world!" << endl;
    log(LogLevel::Info) << "Wrote Hello, world!";
    string str;
    getline(p1.output(), str);
    log(LogLevel::Info) << "Got line: " << str;
    Sleep::duration(seconds(1));
    log(LogLevel::Info) << "Running: " << p1.running();
    p1.closeInput();
    Sleep::duration(seconds(1));
    log(LogLevel::Info) << "Now running: " << p1.running() << endl
                        << "Exit code: " << p1.getExitCode();

    UniqueProcess p2(false, false, false, "/bin/cat", "/proc/version");

    UniqueProcess p3(false, false, false, "/bin/uname", "-a");

    UniqueProcess p4(false, false, false, "/bin/false");
    p4.wait();
    log(LogLevel::Info) << "/bin/false: " << p4.getExitCode();

    UniqueProcess p5(false, false, false, "/bin/bash", "-c", "echo Hello >&2");

    Sleep::duration(seconds(60));
}
