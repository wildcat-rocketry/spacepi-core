#ifndef SPACEPI_TARGETLIB_LINUX_PARTITIONTABLE_HPP
#define SPACEPI_TARGETLIB_LINUX_PARTITIONTABLE_HPP

#include <ostream>
#include <string>
#include <vector>
#include <spacepi/liblinux/Partition.hpp>

namespace spacepi {
    namespace liblinux {
        class PartitionTable {
            public:
                const std::string &getSize() const noexcept;
                PartitionTable &setSize(const std::string &size) noexcept;

                const std::string &getUnit() const noexcept;
                PartitionTable &setUnit(const std::string &unit) noexcept;

                const std::string &getLabel() const noexcept;
                PartitionTable &setLabel(const std::string &label) noexcept;

                const std::string &getLabelID() const noexcept;
                PartitionTable &setLabelID(const std::string &labelID) noexcept;

                const std::string &getFirstLBA() const noexcept;
                PartitionTable &setFirstLBA(const std::string &firstLBA) noexcept;

                const std::string &getLastLBA() const noexcept;
                PartitionTable &setLastLBA(const std::string &lastLBA) noexcept;

                const std::string &getTableLength() const noexcept;
                PartitionTable &setTableLength(const std::string &tableLength) noexcept;

                const std::string &getGrain() const noexcept;
                PartitionTable &setGrain(const std::string &grain) noexcept;

                const std::vector<Partition> &getPartitions() const noexcept;
                PartitionTable &addPartition(const Partition &partition) noexcept;

                std::ostream &printSfdisk(std::ostream &os) const;
                std::ostream &printFstab(std::ostream &os) const;

            private:
                std::string size;
                std::string unit;
                std::string label;
                std::string labelID;
                std::string firstLBA;
                std::string lastLBA;
                std::string tableLength;
                std::string grain;
                std::vector<Partition> partitions;
        };
    }
}

#endif
