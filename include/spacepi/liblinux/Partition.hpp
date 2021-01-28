#ifndef SPACEPI_TARGETLIB_LINUX_PARTITION_HPP
#define SPACEPI_TARGETLIB_LINUX_PARTITION_HPP

#include <ostream>
#include <string>

namespace spacepi {
    namespace liblinux {
        class Partition {
            public:
                const std::string &getStart() const noexcept;
                Partition &setStart(const std::string &start) noexcept;

                const std::string &getSize() const noexcept;
                Partition &setSize(const std::string &size) noexcept;
                
                bool isBootable() const noexcept;
                Partition &setBootable(bool bootable) noexcept;

                const std::string &getAttr() const noexcept;
                Partition &setAttr(const std::string &attr) noexcept;

                const std::string &getUUID() const noexcept;
                Partition &setUUID(const std::string &uuid) noexcept;

                const std::string &getName() const noexcept;
                Partition &setName(const std::string &name) noexcept;

                const std::string &getType() const noexcept;
                Partition &setType(const std::string &type) noexcept;

                const std::string &getMountPoint() const noexcept;
                Partition &setMountPoint(const std::string &mountPoint) noexcept;

                const std::string &getFSType() const noexcept;
                Partition &setFSType(const std::string &fsType) noexcept;

                const std::string &getFormatOptions() const noexcept;
                Partition &setFormatOptions(const std::string &formatOptions) noexcept;

                const std::string &getOptions() const noexcept;
                Partition &setOptions(const std::string &options) noexcept;

                int getDump() const noexcept;
                Partition &setDump(int dump) noexcept;

                int getPass() const noexcept;
                Partition &setPass(int pass) noexcept;

                std::ostream &printSfdisk(std::ostream &os) const;
                std::ostream &printFstab(std::ostream &os) const;

            private:
                std::string start;
                std::string size;
                bool bootable;
                std::string attr;
                std::string uuid;
                std::string name;
                std::string type;
                std::string mountPoint;
                std::string fsType;
                std::string formatOptions;
                std::string options;
                int dump;
                int pass;
        };
    }
}

#endif
