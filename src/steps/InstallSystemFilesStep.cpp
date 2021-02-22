#include <fstream>
#include <ios>
#include <string>
#include <boost/filesystem.hpp>
#include <spacepi/liblinux/steps/InstallSystemFilesStep.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/PartitionTable.hpp>
#include <spacepi/liblinux/SharedTempDir.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void InstallSystemFilesStep::run(InstallationData &data) {
    path root = data.getData<SharedTempDir>().getPath();
    PartitionTable &tab = data.getData<PartitionTable>();
    // /bin/sh
    create_directories(root / "bin");
    remove(root / "bin/sh");
    create_symlink("bash", root / "bin/sh");
    // /etc/apt/apt.conf.d/50ro-root
    create_directories(root / "etc/apt/apt.conf.d");
    std::ofstream((root / "etc/apt/apt.conf.d/50ro-root").native()) <<
        "DPkg {\n"
        "    Pre-Invoke { \"mount -o remount,rw /\"; };\n"
        "    Post-Invoke { \"mount -o remount,ro / || true\"; };\n"
        "};\n";
    // /etc/environment
    std::ofstream((root / "etc/environment").native()) <<
        "BLKID_FILE=/var/local/etc/blkid.tab\n";
    // /etc/fstab
    {
        std::ofstream ofs((root / "etc/fstab").native());
        tab.printFstab(ofs);
    }
    // /etc/hostname
    std::ofstream((root / "etc/hostname").native()) <<
        "spacepi-unconfigured\n";
    // /etc/mtab
    remove(root / "etc/mtab");
    create_symlink("/proc/self/mounts", root / "etc/mtab");
    // /etc/NetworkManager/system-connections/ethernet.nmconnection
    create_directories(root / "etc/NetworkManager/system-connections");
    std::ofstream((root / "etc/NetworkManager/system-connections/ethernet.nmconnection").native()) <<
        "[connection]\n"
        "id=ethernet\n"
        "uuid=d7d62e7f-c407-41cc-8914-61bb11a19719\n"
        "type=802-3-ethernet\n"
        "\n"
        "[802-3-ethernet]\n"
        "\n"
        "[ipv4]\n"
        "method=auto\n"
        "\n"
        "[ipv6]\n"
        "method=auto\n"
        "ip6-privacy=2\n";
    // /etc/resolv.conf
    std::ofstream((root / "etc/resolv.conf").native()) <<
        "nameserver 1.1.1.1\n"
        "nameserver 8.8.8.8\n";
    // /etc/sudoers
    {
        std::ifstream in((root / "etc/sudoers").native());
        std::ofstream out((root / "etc/sudoers~").native());
        string line;
        while (getline(in, line)) {
            if (line.substr(0, 5) == "sudo\t") {
                out << "sudo\tALL=(ALL:ALL) NOPASSWD:ALL\n";
            } else {
                out << line << "\n";
            }
        }
    }
    rename(root / "etc/sudoers~", root / "etc/sudoers");
    // /home
    remove(root / "home");
    create_directory_symlink("/var/local/home", root / "home");
    // /lib/systemd/system/getty@.service
    create_directories(root / "lib/systemd/system");
    {
        std::ifstream in((root / "lib/systemd/system/getty@.service").native());
        std::ofstream out((root / "lib/systemd/system/getty@.service~").native());
        string line;
        while (getline(in, line)) {
            if (line.substr(0, 17) == "TTYVTDisallocate=") {
                out << "TTYVTDisallocate=no\n";
            } else {
                out << line << "\n";
            }
        }
    }
    rename(root / "lib/systemd/system/getty@.service~", root / "lib/systemd/system/getty@.service");
    // /var/local/etc
    create_directories(root / "var/local/etc");
    // /var/local/home
    create_directories(root / "var/local/home");
}
