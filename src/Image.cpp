#include <fstream>
#include <SpacePi.hpp>
#include <spacepi/liblinux/Config.hpp>
#include <spacepi/liblinux/Image.hpp>
#include <spacepi/liblinux/Partition.hpp>
#include <spacepi/liblinux/PartitionTable.hpp>
#include <spacepi/liblinux/SharedMount.hpp>
#include <spacepi/liblinux/UniqueProcess.hpp>
#include <string>
#include <vector>

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
    std::string size = tab.getSize();
    std::streampos bigBoi = stoull(size);
    switch(size.back()){
        case 'K':
        case 'k':
            bigBoi = bigBoi*1024-1;
            break;
        case 'M':
        case 'm':
            bigBoi = bigBoi*1024*1024-1;
            break;
        case 'G':
        case 'g':
            bigBoi = bigBoi*1024*1024*1024-1;
            break;
        default:
            throw EXCEPTION(ArgumentException("Partition size invalid."));
            break;
    }
    ofs.seekp(bigBoi);
    ofs << '\0';
    ofs.close();

    UniqueProcess sfdisk(true,false,false,SFDISK_EXECUTABLE,getFilename());
    tab.printSfdisk(sfdisk.input());
    sfdisk.closeInput();
    sfdisk.wait();
    if(sfdisk.getExitCode() != 0){
        throw EXCEPTION(ResourceException("Error writing image."));
    }
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
