#ifndef SPACEPI_TARGETLIB_LINUX_IMAGE_HPP
#define SPACEPI_TARGETLIB_LINUX_IMAGE_HPP

#include <spacepi/liblinux/Partition.hpp>
#include <spacepi/liblinux/PartitionTable.hpp>
#include <spacepi/liblinux/SharedMount.hpp>
#include <spacepi/liblinux/SharedLoopDevice.hpp>
#include <string>
#include <utility>
#include <vector>

namespace spacepi {
    namespace liblinux {
        class Image {
            public:
                Image(const std::string &filename) noexcept;

                const std::string &getFilename() const noexcept;

                void formatPartitions(const PartitionTable &tab);

                SharedMount mountPartitionAt(int partNo, const std::string &fsType, const std::string &options, const std::string &mountDir);
                SharedMount mountPartitionAt(int partNo, const Partition &part, const std::string &mountDir);
                SharedMount mountPartition(int partNo, const Partition &part, const std::string &rootDir);
                std::vector<SharedMount> mountPartitions(const PartitionTable &tab, const std::string &rootDir);

            private:
                static bool sortMountOrder(const std::pair<Partition, int> &a, const std::pair<Partition, int> &b) noexcept;

                std::string filename;
                SharedLoopDevice loopDevice;
        };
    }
}

#endif
