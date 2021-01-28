#include <string>
#include <string>
#include <spacepi/liblinux/Partition.hpp>

using namespace spacepi::liblinux;

const std::string &Partition::getStart() const noexcept{
    return start;
}

Partition &Partition::setStart(const std::string &start) noexcept{
    this->start = start;
    return *this;
}

const std::string &Partition::getSize() const noexcept{
    return size;
}

Partition &Partition::setSize(const std::string &size) noexcept{
    this->size = size;
    return *this;
}

bool Partition::isBootable() const noexcept{
    return bootable;
}

Partition &Partition::setBootable(bool bootable) noexcept{
    this->bootable = bootable;
    return *this;
}

const std::string &Partition::getAttr() const noexcept{
    return attr;
}

Partition &Partition::setAttr(const std::string &attr) noexcept{
    this->attr = attr;
    return *this;
}

const std::string &Partition::getUUID() const noexcept{
    return uuid;
}

Partition &Partition::setUUID(const std::string &uuid) noexcept{
    this->uuid = uuid;
    return *this;
}

const std::string &Partition::getName() const noexcept{
    return name;
}

Partition &Partition::setName(const std::string &name) noexcept{
    this->name = name;
    return *this;
}

const std::string &Partition::getType() const noexcept{
    return type;
}

Partition &Partition::setType(const std::string &type) noexcept{
    this->type = type;
    return *this;
}

const std::string &Partition::getMountPoint() const noexcept{
    return mountPoint;
}

Partition &Partition::setMountPoint(const std::string &mountPoint) noexcept{
    this->mountPoint = mountPoint;
    return *this;
}

const std::string &Partition::getFSType() const noexcept{
    return fsType;
}

Partition &Partition::setFSType(const std::string &fsType) noexcept{
    this->fsType;
    return *this;
}

const std::string &Partition::getFormatOptions() const noexcept{
    return formatOptions;
}

Partition &Partition::setFormatOptions(const std::string &formatOptions) noexcept{
    this->formatOptions;
    return *this;
}

const std::string &Partition::getOptions() const noexcept{
    return options;
}

Partition &Partition::setOptions(const std::string &options) noexcept{
    this->options = options;
    return *this;
}

int Partition::getDump() const noexcept{
    return dump;
}

Partition &Partition::setDump(int dump) noexcept{
    this->dump = dump;
    return *this;
}

int Partition::getPass() const noexcept{
    return pass;
}

Partition &Partition::setPass(int pass) noexcept{
    this->pass = pass;
    return *this;
}

std::ostream &Partition::printSfdisk(std::ostream &os) const{
    return os;
}

std::ostream &Partition::printFstab(std::ostream &os) const{
    os << "UUID=" << uuid << " ";
    os << mountPoint << " ";
    os << fsType << " ";
    os << options << " ";
    os << dump << " ";
    os << pass;
    return os;
}
