#include <blkid/blkid.h>
#include <boost/filesystem.hpp>
#include <fstream>
#include <set>
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
    set<Partition, detail::PartitionSorter> &parts = tab.getPartitions();
    for (set<Partition, detail::PartitionSorter>::iterator it = parts.begin(); it != parts.end(); ++it) {
        Partition part = *it;
        if (part.getNumber() < 0) {
            continue;
        }
        string block = loopDevice.getBlockDevice(part.getNumber());
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
        handle(!!pr)
            << "Unable to create BLKID probe for " << block << ": " << SyscallErrorString;
        handle(blkid_probe_enable_partitions(pr, 1))
            << "Unable to enable partitions on BLKID probe for " << block << ": " << SyscallErrorString;
        handle(blkid_probe_set_partitions_flags(pr, BLKID_PARTS_ENTRY_DETAILS))
            << "Unable to set partition flags on BLKID probe for " << block << ": " << SyscallErrorString;
        handle(blkid_do_fullprobe(pr))
            << "Unable to run BLKID probe for " << block << ": " << SyscallErrorString;
        const char *uuid = "";
        blkid_probe_lookup_value(pr, "UUID", &uuid, nullptr);
        part.setUUID(string(uuid));
        uuid = "";
        blkid_probe_lookup_value(pr, "PART_ENTRY_UUID", &uuid, nullptr);
        part.setPartUUID(string(uuid));
        blkid_free_probe(pr);
        it = parts.insert(parts.erase(it), part);
    }
}

vector<SharedMount> Image::mountPartitions(const PartitionTable &tab, const std::string &rootDir, bool forceRW) {
    if(!loopDevice){
        loopDevice = SharedLoopDevice(getFilename());
    }
    vector<SharedMount> mounts;
    const set<Partition, detail::PartitionSorter> &parts = tab.getPartitions();
    mounts.reserve(parts.size());
    for (set<Partition, detail::PartitionSorter>::const_iterator it = parts.begin(); it != parts.end(); ++it) {
        std::string source;
        if (it->getNumber() >= 0) {
            source = loopDevice.getBlockDevice(it->getNumber());
        } else {
            source = it->getSource();
        }
        string mountDir = rootDir + it->getMountPoint();
        create_directories(mountDir);
        SharedMount mnt(source, mountDir, it->getOptions(), it->getFSType());
        if (forceRW) {
            mnt.remount("rw");
        }
        mounts.push_back(mnt);
    }
    return mounts;
}
