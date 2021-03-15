#include <chrono>
#include <cstdio>
#include <cstring>
#include <exception>
#include <string>
#include <vector>
#include <linux/fiemap.h>
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/CloneImageStep.hpp>
#include <spacepi/liblinux/Image.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationOptions.hpp>
#include <spacepi/liblinux/ResourceTempDir.hpp>
#include <spacepi/liblinux/SystemCaller.hpp>

using namespace std;
using namespace std::chrono;
using namespace spacepi::log;
using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void CloneImageStep::run(InstallationData &data) {
    InstallationOptions &installData = data.getData<InstallationOptions>();
    data.initData<ResourceTempDir>("clone");
    const string &tempDir = data.getData<ResourceTempDir>().getPath();
    string clone = tempDir + "/image.iso";

    int in = open(installData.getOutFile().c_str(), O_RDONLY);
    handle(in)
        << "Could not open output file for reading: " << SyscallErrorString << ".  Is the image already built?";
    try {
        off_t size = lseek(in, 0, SEEK_END);
        if (size < 0) {
            handle(size)
                << "Error seeking output file: " << SyscallErrorString;
        }
        vector<char> mapBuf;
        mapBuf.resize(sizeof(struct fiemap));
        struct fiemap *map = (struct fiemap *) mapBuf.data();
        map->fm_length = size;
        handle(ioctl(in, FS_IOC_FIEMAP, map))
            << "Error creating fie map for image: " << SyscallErrorString;
        mapBuf.resize(sizeof(struct fiemap) + map->fm_mapped_extents * sizeof(struct fiemap_extent));
        map = (struct fiemap *) mapBuf.data();
        map->fm_extent_count = map->fm_mapped_extents;
        handle(ioctl(in, FS_IOC_FIEMAP, map))
            << "Error creating fie map for image: " << SyscallErrorString;
        ssize_t total = 0;
        for (int i = 0; i < map->fm_mapped_extents; ++i) {
            total += map->fm_extents[i].fe_length;
        }
        int out = open(clone.c_str(), O_WRONLY | O_CREAT);
        handle(out)
            << "Could not open file to clone image to: " << SyscallErrorString;
        try {
            off_t ret = lseek(out, size - 1, SEEK_SET);
            if (ret < 0) {
                handle(ret)
                    << "Could not seek to end of output file: " << SyscallErrorString;
            }
            handle(write(out, "", 1))
                << "Could not resize output file: " << SyscallErrorString;
            char buffer[BUFSIZ];
            ssize_t copied = 0;
            steady_clock::time_point lastMessage = steady_clock::now();
            for (int i = 0; i < map->fm_mapped_extents; ++i) {
                ret = lseek(in, map->fm_extents[i].fe_logical, SEEK_SET);
                if (ret < 0) {
                    handle(ret)
                        << "Could not seek input file: " << SyscallErrorString;
                }
                ret = lseek(out, map->fm_extents[i].fe_logical, SEEK_SET);
                if (ret < 0) {
                    handle(ret)
                        << "Could not seek output file: " << SyscallErrorString;
                }
                ssize_t complete = 0;
                while (complete < map->fm_extents[i].fe_length) {
                    ssize_t count = read(in, buffer, sizeof(buffer));
                    handle(count)
                        << "Error reading input file: " << SyscallErrorString;
                    if (count == 0) {
                        throw EXCEPTION(ResourceException("Input file extents changed after mapping."));
                    }
                    handle(write(out, buffer, count));
                    copied += count;
                    complete += count;
                    steady_clock::time_point now = steady_clock::now();
                    if (now - lastMessage > seconds(1)) {
                        lastMessage += seconds(1);
                        log(LogLevel::Info) << "Cloning image: " << (100 * copied / total) << "%";
                    }
                }
            }
        } catch (const exception &e) {
            close(out);
            throw;
        }
        close(out);
    } catch (const exception &e) {
        close(in);
        throw;
    }
    close(in);
    log(LogLevel::Info) << "Cloning image: 100%, done";

    data.initData<Image>(clone);
}
