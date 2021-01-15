#include <cstdint>
#include <fstream>
#include <string>
#include <linux/magic.h>
#include <sys/vfs.h>
#include <SpacePi.hpp>
#include <spacepi/target/extension/Memory.hpp>

using namespace std;
using namespace spacepi;
using namespace spacepi::util;
using namespace spacepi::target::extension;

#define MOUNTS_FILE "/proc/mounts"
#define MEM_FILE "/proc/meminfo"

resource::MemoryInfo Memory::getDiskInfo() {
    ifstream ifs(MOUNTS_FILE);
    if (!ifs) {
        throw EXCEPTION(ResourceException("Unable to open " MOUNTS_FILE));
    }
    uint64_t free = 0;
    uint64_t total = 0;
    bool allFree = true;
    while (!ifs.eof()) {
        string device;
        string target;
        string type;
        string flags;
        int dump;
        int pass;
        ifs >> device >> target >> type >> flags >> dump >> pass;
        if (ifs.bad()) {
            throw EXCEPTION(ResourceException("Could not read " MOUNTS_FILE));
        } else if (ifs.fail()) {
            throw EXCEPTION(ResourceException(MOUNTS_FILE " is incorrectly formatted"));
        }
        if (device.front() == '/') {
            struct statfs stats;
            if (statfs(target.c_str(), &stats) < 0) {
                throw EXCEPTION(ResourceException("Unable to stat " + target));
            }
            free += stats.f_bfree * stats.f_bsize;
            total += stats.f_blocks * stats.f_bsize;
            if (stats.f_bfree == 0 || stats.f_bavail == 0 || (stats.f_type != MSDOS_SUPER_MAGIC && stats.f_ffree == 0)) {
                allFree = false;
            }
        }
    }
    if (!allFree) {
        free = 0;
    }
    return resource::MemoryInfo(free, total);
}

resource::RAMInfo Memory::getRAMInfo() {
    ifstream ifs(MEM_FILE);
    if (!ifs) {
        throw EXCEPTION(ResourceException("Unable to open " MEM_FILE));
    }
    uint64_t memTotal = 0;
    uint64_t memFree = 0;
    uint64_t swapTotal = 0;
    uint64_t swapFree = 0;
    string key = "";
    while (!ifs.eof() && (memTotal == 0 || memFree == 0 || swapTotal == 0 || swapFree == 0)) {
        uint64_t val;
        string unit;
        if (key.empty()) {
            ifs >> key;
        }
        ifs >> val >> unit;
        if (ifs.bad()) {
            throw EXCEPTION(ResourceException("Could not read " MEM_FILE));
        } else if (ifs.fail()) {
            throw EXCEPTION(ResourceException(MEM_FILE " is incorrectly formatted"));
        }
        string nextKey = "";
        if (unit == "kB") {
            val *= 1024;
        } else {
            nextKey = unit;
        }
        if (key == "MemTotal:") {
            memTotal = val;
        } else if (key == "MemFree:") {
            memFree = val;
        } else if (key == "SwapTotal:") {
            swapTotal = val;
        } else if (key == "SwapFree:") {
            swapFree = val;
        }
        key = nextKey;
    }
    return resource::RAMInfo(resource::MemoryInfo(memFree, memTotal), resource::MemoryInfo(swapFree, swapTotal));
}
