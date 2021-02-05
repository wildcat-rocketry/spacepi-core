#include <string>
#include <vector>
#include <fstream>
#include <SpacePi.hpp>
#include <spacepi/liblinux/Image.hpp>
#include <spacepi/liblinux/Partition.hpp>
#include <spacepi/liblinux/PartitionTable.hpp>
#include <spacepi/liblinux/SharedMount.hpp>
#include <spacepi/liblinux/UniqueProcess.hpp>

using namespace spacepi::liblinux;
using namespace spacepi::util;
using namespace std;

Image::Image(const string &filename) noexcept : filename(filename) {
}

const string &Image::getFilename() const noexcept {
    return filename;
}

void Image::formatPartitions(const PartitionTable &tab) {
    std::ofstream ofs (getFilename(),std::ofstream::out | std::ostream::binary);
    std::streampos bigBoi = 16L*1024*1024*1024-1; //16GB
    ofs.seekp(bigBoi);
    ofs << '\0';
    ofs.close();

    //UniqueProcess sfdisk(true,false,false,"/sbin/sfdisk",getFilename());
    UniqueProcess sfdisk(true,false,false,"/bin/cat", "--");
    tab.printSfdisk(sfdisk.input());
    /*{
    spacepi::log::LogStream s = spacepi::log::Logger("test")(spacepi::log::LogLevel::Info);
    tab.printSfdisk(s);
    }*/
    sfdisk.closeInput();
    sfdisk.wait();
}

SharedMount Image::mountPartitionAt(int partNo, const string &fsType, const string &options, const string &mountDir) {
    throw EXCEPTION(StateException("Not implemented."));
}

SharedMount Image::mountPartitionAt(int partNo, const Partition &part, const std::string &mountDir) {
    return mountPartitionAt(partNo, part.getFSType(), part.getOptions(), mountDir);
}

SharedMount Image::mountPartition(int partNo, const Partition &part, const std::string &rootDir) {
    return mountPartitionAt(partNo, part, rootDir + part.getMountPoint());
}

vector<SharedMount> Image::mountPartitions(const PartitionTable &tab, const std::string &rootDir) {
    vector<SharedMount> mounts;
    const vector<Partition> &parts = tab.getPartitions();
    mounts.reserve(parts.size());
    int i = 0;
    for (vector<Partition>::const_iterator it = parts.begin(); it != parts.end(); ++it) {
        mounts.push_back(mountPartition(i, *it, rootDir));
        ++i;
    }
    return mounts;
}
