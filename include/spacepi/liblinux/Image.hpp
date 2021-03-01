#ifndef SPACEPI_TARGETLIB_LINUX_IMAGE_HPP
#define SPACEPI_TARGETLIB_LINUX_IMAGE_HPP

#include <spacepi/liblinux/Partition.hpp>
#include <spacepi/liblinux/PartitionTable.hpp>
#include <spacepi/liblinux/SharedMount.hpp>
#include <spacepi/liblinux/SharedLoopDevice.hpp>
#include <spacepi/liblinux/SystemCaller.hpp>
#include <string>
#include <utility>
#include <vector>

namespace spacepi {
    namespace liblinux {
        class Image : private SystemCaller {
            public:
                Image(const std::string &filename) noexcept;

                const std::string &getFilename() const noexcept;

                void formatPartitions(PartitionTable &tab);

                std::vector<SharedMount> mountPartitions(const PartitionTable &tab, const std::string &rootDir, bool forceRW = false);

            private:
                std::string filename;
                SharedLoopDevice loopDevice;
        };
    }
}

#endif
