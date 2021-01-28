#include <ostream>
#include <string>
#include <vector>
#include <spacepi/liblinux/Partition.hpp>
#include <spacepi/liblinux/PartitionTable.hpp>

using namespace spacepi::liblinux;

const std::string &PartitionTable::getSize() const noexcept{
    return size;
}

PartitionTable &PartitionTable::setSize(const std::string &size) noexcept{
    this-> size = size;
    return *this;
}

const std::string &PartitionTable::getUnit() const noexcept{
    return unit;
}

PartitionTable &PartitionTable::setUnit(const std::string &unit) noexcept{
    this->unit = unit;
    return *this;
}

const std::string &PartitionTable::getLabel() const noexcept{
    return label;
}

PartitionTable &PartitionTable::setLabel(const std::string &label) noexcept{
    this->label = label;
    return *this;
}

const std::string &PartitionTable::getLabelID() const noexcept{
    return labelID;
}

PartitionTable &PartitionTable::setLabelID(const std::string &labelID) noexcept{
    this->labelID = labelID;
    return *this;
}

const std::string &PartitionTable::getFirstLBA() const noexcept{
    return firstLBA;
}

PartitionTable &PartitionTable::setFirstLBA(const std::string &firstLBA) noexcept{
    this->firstLBA;
    return *this;
}

const std::string &PartitionTable::getLastLBA() const noexcept{
    return lastLBA;
}

PartitionTable &PartitionTable::setLastLBA(const std::string &lastLBA) noexcept{
    this->lastLBA;
    return *this;
}

const std::string &PartitionTable::getTableLength() const noexcept{
    return tableLength;
}

PartitionTable &PartitionTable::setTableLength(const std::string &tableLength) noexcept{
    this->tableLength = tableLength;
    return *this;
}

const std::string &PartitionTable::getGrain() const noexcept{
    return grain;
}

PartitionTable &PartitionTable::setGrain(const std::string &grain) noexcept{
    this->grain = grain;
    return *this;
}

const std::vector<Partition> &PartitionTable::getPartitions() const noexcept{
    return partitions;
}

PartitionTable &PartitionTable::addPartition(const Partition &partition) noexcept{
    partitions.push_back(partition);
    return *this;
}

std::ostream &PartitionTable::printSfdisk(std::ostream &os) const{
    return os;
}

std::ostream &PartitionTable::printFstab(std::ostream &os) const{
    for(int i = 0; i < partitions.size(); i++){
        partitions[i].printFstab(os) << "\n";
    }
    return os;
}
