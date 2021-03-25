#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iomanip>
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
    // /etc/exports
    {
        std::ofstream ofs((root / "etc/exports").native());
        ofs << "# /etc/exports: the access control list for filesystems which may be exported\n"
               "#               to NFS clients.  See exports(5).\n"
               "#\n"
               "# <path>                        <hosts>\n" << left;
        for (vector<string>::const_iterator it = config.nfsDirs.begin(); it != config.nfsDirs.end(); ++it) {
            ofs << setw(31) << *it << " localhost(insecure,rw,sync,no_subtree_check,no_root_squash)\n";
        }
    }
    // /etc/fstab
    {
        std::ofstream ofs((root / "etc/fstab").native());
        tab.printFstab(ofs);
    }
    // /etc/gitconfig
    std::ofstream((root / "etc/gitconfig").native()) <<
        "[alias]\n"
        "\tupdate = \"!f(){ git pull \\\"$@\\\" && git submodule update --init --recursive --remote; };f\"\n"
        "\tset-master = \"!f(){ git tag tmp master && git branch -d master && git checkout -b master && git merge tmp && git tag -d tmp && git branch -u origin master; };f\"\n";
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
    // /etc/pam.d/common-session
    create_directories(root / "etc/pam.d");
    std::ofstream((root / "etc/pam.d/common-session").native(), ios::out | ios::app) <<
        "\n"
        "# SpacePi rules\n"
        "session optional pam_umask.so umask=002\n";
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
    // /etc/skel/.ssh
    create_directories(root / "etc/skel/.ssh");
    // /etc/skel/.ssh/id_rsa
    create_symlink("/etc/ssh/id_rsa", root / "etc/skel/.ssh/id_rsa");
    // /etc/skel/.ssh/id_rsa.pub
    create_symlink("/etc/ssh/id_rsa.pub", root / "etc/skel/.ssh/id_rsa.pub");
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
    std::ofstream((root / "etc/sudoers").native(), ios::out | ios::app) <<
        "\n"
        "# SpacePi privilege specification\n"
        "%spacepi ALL=(ALL:ALL) NOPASSWD:ALL\n";
    // /etc/tmpfiles.d/spacepi.conf
    std::ofstream((root / "etc/tmpfiles.d/spacepi.conf").native()) <<
        "d /run 1777 root root\n";
    // /etc/udev/rules.d/80-spacepi.rules
    std::ofstream((root / "etc/udev/rules.d/80-spacepi.rules").native()) <<
        "KERNEL==\"ttyACM*\",  OWNER=\"root\", GROUP=\"spacepi\", MODE=\"0660\"\n"
        "KERNEL==\"ttyAMA*\",  OWNER=\"root\", GROUP=\"spacepi\", MODE=\"0660\"\n"
        "KERNEL==\"ttyS*\",    OWNER=\"root\", GROUP=\"spacepi\", MODE=\"0660\"\n"
        "KERNEL==\"ttyUSB*\",  OWNER=\"root\", GROUP=\"spacepi\", MODE=\"0660\"\n"
        "SUBSYSTEMS==\"gpio\", OWNER=\"root\", GROUP=\"spacepi\", MODE=\"0660\"\n"
        "SUBSYSTEMS==\"i2c\",  OWNER=\"root\", GROUP=\"spacepi\", MODE=\"0660\"\n"
        "SUBSYSTEMS==\"spi\",  OWNER=\"root\", GROUP=\"spacepi\", MODE=\"0660\"\n";
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
