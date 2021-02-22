#include <algorithm>
#include <blkid/blkid.h>
#include <boost/filesystem.hpp>
#include <fstream>
#include <SpacePi.hpp>
#include <spacepi/liblinux/Config.hpp>
#include <spacepi/liblinux/Config.hpp>
#include <spacepi/liblinux/Image.hpp>
#include <spacepi/liblinux/Partition.hpp>
#include <spacepi/liblinux/PartitionTable.hpp>
#include <spacepi/liblinux/SharedLoopDevice.hpp>
#include <spacepi/liblinux/SharedMount.hpp>
#include <spacepi/liblinux/UniqueLoopDevice.hpp>
#include <spacepi/liblinux/UniqueProcess.hpp>
#include <string>
#include <utility>
#include <vector>

using namespace boost::filesystem;
using namespace spacepi::liblinux;
using namespace spacepi::util;
using namespace std;

Image::Image(const string &filename) noexcept : filename(filename) {
}

const string &Image::getFilename() const noexcept {
    return filename;
}

void Image::formatPartitions(PartitionTable &tab) {
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

    UniqueProcess sfdisk(true,false,false,SFDISK_EXECUTABLE,{ getFilename() });
    tab.printSfdisk(sfdisk.input());
    sfdisk.closeInput();
    sfdisk.wait();
    if(sfdisk.getExitCode() != 0){
        throw EXCEPTION(ResourceException("Error writing image."));
    }
    loopDevice = SharedLoopDevice(getFilename());
    for(int i = 0; i < tab.getPartitions().size(); i++){
        Partition &part = tab.getPartitions()[i];
        string block = loopDevice.getBlockDevice(i);
        vector<string> args;
        const vector<string> &formatOpts = part.getFormatOptions();
        args.reserve(3 + formatOpts.size());
        args.push_back("-t");
        args.push_back(part.getFSType());
        for (vector<string>::const_iterator it = formatOpts.begin(); it != formatOpts.end(); ++it) {
            args.push_back(*it);
        }
        args.push_back(block);
        UniqueProcess process(false,false,false,MKFS_EXECUTABLE,args);
        process.wait();
        if(process.getExitCode() != 0){
            throw EXCEPTION(ResourceException("Error formatting."));
        }
        blkid_probe pr = blkid_new_probe_from_filename(block.c_str());
        if (!pr) {
            throw EXCEPTION(ResourceException("Unable to create BLKID probe for " + block));
        }
        blkid_do_probe(pr);
        const char *uuid;
        blkid_probe_lookup_value(pr, "UUID", &uuid, NULL);
        part.setUUID(string(uuid));
    }
}

SharedMount Image::mountPartitionAt(int partNo, const string &fsType, const string &options, const string &mountDir) {
    if(!loopDevice){
        loopDevice = SharedLoopDevice(getFilename());
    }
    create_directories(mountDir);
    return SharedMount(loopDevice.getBlockDevice(partNo),mountDir,options,fsType);
}

SharedMount Image::mountPartitionAt(int partNo, const Partition &part, const std::string &mountDir) {
    return mountPartitionAt(partNo, part.getFSType(), part.getOptions(), mountDir);
}

SharedMount Image::mountPartition(int partNo, const Partition &part, const std::string &rootDir) {
    return mountPartitionAt(partNo, part, rootDir + part.getMountPoint());
}

vector<SharedMount> Image::mountPartitions(const PartitionTable &tab, const std::string &rootDir, bool forceRW) {
    vector<SharedMount> mounts;
    const vector<Partition> &rawParts = tab.getPartitions();
    vector<pair<Partition, int>> parts;
    parts.reserve(rawParts.size());
    int i = 0;
    for (vector<Partition>::const_iterator it = rawParts.begin(); it != rawParts.end(); ++it) {
        parts.emplace_back(*it, i++);
    }
    sort(parts.begin(), parts.end(), sortMountOrder);
    mounts.reserve(parts.size());
    for (vector<pair<Partition, int>>::const_iterator it = parts.begin(); it != parts.end(); ++it) {
        SharedMount mnt = mountPartition(it->second, it->first, rootDir);
        if (forceRW) {
            mnt.remount("rw");
        }
        mounts.push_back(mnt);
    }
    return mounts;
}

bool Image::sortMountOrder(const pair<Partition, int> &a, const pair<Partition, int> &b) noexcept {
    return a.first.getMountPoint().size() < b.first.getMountPoint().size();
}
