#include <cstddef>
#include <cstdio>
#include <fstream>
#include <ios>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/InstallSystemFilesStep.hpp>
#include <spacepi/liblinux/Config.hpp>
#include <spacepi/liblinux/InstallationConfig.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/PartitionTable.hpp>
#include <spacepi/liblinux/SharedTempDir.hpp>
#include <spacepi/liblinux/UniqueProcess.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void InstallSystemFilesStep::run(InstallationData &data) {
    path root = data.getData<SharedTempDir>().getPath();
    PartitionTable &tab = data.getData<PartitionTable>();
    InstallationConfig &config = data.getData<InstallationConfig>();
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
    // /etc/hosts
    std::ofstream((root / "etc/hosts").native()) <<
        "# /etc/hosts: static DNS resolution information.\n"
        "#\n"
        "# <ip address>  <host name>\n"
        "127.0.0.1       localhost\n"
        "127.0.0.1       spacepi-unconfigured\n";
    // /etc/modules
    std::ofstream((root / "etc/modules").native()) <<
        "# /etc/modules: kernel modules to load at boot time.\n"
        "#\n"
        "# This file contains the names of kernel modules that should be loaded\n"
        "# at boot time, one per line. Lines beginning with \"#\" are ignored.\n"
        "i2c-dev\n";
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
    // /etc/passwd
    UniqueProcess useradd(false, false, false, USERADD_EXECUTABLE, { "-d", "/", "-M", "-r", "-R", root.native(), "-U", "spacepi" });
    useradd.wait();
    if (useradd.getExitCode() != 0) {
        throw EXCEPTION(ResourceException("Could not create SpacePi user."));
    }
    {
        std::ifstream pwd((root / "etc/passwd").native());
        string line;
        bool found = false;
        while (getline(pwd, line)) {
            if (line.substr(0, 8) == "spacepi:") {
                int uid, gid;
                if (sscanf(line.c_str(), "spacepi:%*[^:]:%d:%d:", &uid, &gid) != 2) {
                    throw EXCEPTION(ResourceException("Incorrect format in /etc/passwd"));
                }
                config.sourceUid = (uid_t) uid;
                config.sourceGid = (gid_t) gid;
                found = true;
                break;
            }
        }
        if (!found) {
            throw EXCEPTION(ResourceException("Could not find spacepi user in /etc/passwd"));
        }
    }
    // /etc/resolv.conf
    std::ofstream((root / "etc/resolv.conf").native()) <<
        "nameserver 1.1.1.1\n"
        "nameserver 8.8.8.8\n";
    // /etc/ssh/ssh_host_*
    {
        vector<path> files;
        for (directory_iterator it(root / "etc/ssh"); it != directory_iterator(); ++it) {
            if (it->path().filename().native().substr(0, 9) == "ssh_host_") {
                files.push_back(it->path());
            }
        }
        for (vector<path>::const_iterator it = files.begin(); it != files.end(); ++it) {
            remove(*it);
        }
    }
    // /etc/sudoers
    {
        std::ifstream in((root / "etc/sudoers").native());
        std::ofstream out((root / "etc/sudoers~").native());
        string line;
        while (getline(in, line)) {
            if (line.substr(0, 6) == "%sudo\t") {
                out << "%sudo\tALL=(ALL:ALL) NOPASSWD:ALL\n";
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
