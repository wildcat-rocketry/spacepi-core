#include <fstream>
#include <set>
#include <string>
#include <boost/filesystem.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/Partition.hpp>
#include <spacepi/liblinux/PartitionTable.hpp>
#include <spacepi/liblinux/SharedTempDir.hpp>
#include <spacepi/target/Config.hpp>
#include <spacepi/target/Firmware.hpp>
#include <spacepi/target/InstallFirmwareStep.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::detail;
using namespace spacepi::target;

void InstallFirmwareStep::run(InstallationData &data) {
    path fwRoot = data.getData<Firmware>().getDownloadRepo();
    path root = data.getData<SharedTempDir>().getPath();
    const set<Partition, PartitionSorter> &parts = data.getData<PartitionTable>().getPartitions();
    string rootUUID;
    for (set<Partition, PartitionSorter>::const_iterator it = parts.begin(); it != parts.end(); ++it) {
        if (it->getMountPoint() == "/") {
            rootUUID = it->getPartUUID();
            break;
        }
    }
    // /boot/
    create_directories(root / "boot");
    for (recursive_directory_iterator it(fwRoot / "boot"); it != recursive_directory_iterator(); ++it) {
        copy(*it, root / it->path().native().substr(fwRoot.native().size()));
    }
    // /boot/cmdline.txt
    std::ofstream((root / "boot/cmdline.txt").native()) <<
        "root=PARTUUID=" << rootUUID << " rootwait fsck.repair=yes ro console=tty0 elevator=deadline init=/usr/local/bin/spacepictl\n";
    // /boot/config.txt
    std::ofstream((root / "boot/config.txt").native()) <<
        "dtparam=i2c_arm=on\n"
        "dtparam=spi=on\n"
        "enable_uart=1\n"
        "gpu_mem=16\n"
        "dtoverlay=gpio-line-names\n";
    // /boot/overlays/gpio-line-names.dtbo
    copy(GPIO_LINE_NAMES_DTBO_OUTPUT, root / "boot/overlays/gpio-line-names.dtbo");
    // /lib/firmware/brcm/brcmfmac43430-sdio.txt
    create_directories(root / "lib/firmware/brcm");
    std::ofstream((root / "lib/firmware/brcm/brcmfmac43430-sdio.txt").native()) <<
        "# NVRAM file for BCM943430WLSELG\n"
        "# 2.4 GHz, 20 MHz BW mode\n"
        "\n"
        "# The following parameter values are just placeholders, need to be updated.\n"
        "manfid=0x2d0\n"
        "prodid=0x0726\n"
        "vendid=0x14e4\n"
        "devid=0x43e2\n"
        "boardtype=0x0726\n"
        "boardrev=0x1202\n"
        "boardnum=22\n"
        "macaddr=00:90:4c:c5:12:38\n"
        "sromrev=11\n"
        "boardflags=0x00404201\n"
        "boardflags3=0x08000000\n"
        "xtalfreq=37400\n"
        "#xtalfreq=19200\n"
        "nocrc=1\n"
        "ag0=255\n"
        "aa2g=1\n"
        "ccode=ALL\n"
        "\n"
        "pa0itssit=0x20\n"
        "extpagain2g=0\n"
        "\n"
        "#PA parameters for 2.4GHz, measured at CHIP OUTPUT\n"
        "pa2ga0=-168,7161,-820\n"
        "AvVmid_c0=0x0,0xc8\n"
        "cckpwroffset0=5\n"
        "\n"
        "# PPR params\n"
        "maxp2ga0=84\n"
        "txpwrbckof=6\n"
        "cckbw202gpo=0\n"
        "legofdmbw202gpo=0x66111111\n"
        "mcsbw202gpo=0x77711111\n"
        "propbw202gpo=0xdd\n"
        "\n"
        "# OFDM IIR :\n"
        "ofdmdigfilttype=18\n"
        "ofdmdigfilttypebe=18\n"
        "# PAPD mode:\n"
        "papdmode=1\n"
        "papdvalidtest=1\n"
        "pacalidx2g=32\n"
        "papdepsoffset=-36\n"
        "papdendidx=61\n"
        "\n"
        "il0macaddr=00:90:4c:c5:12:38\n"
        "wl0id=0x431b\n"
        "\n"
        "deadman_to=0xffffffff\n"
        "# muxenab: 0x1 for UART enable, 0x2 for GPIOs, 0x8 for JTAG\n"
        "muxenab=0x1\n"
        "# CLDO PWM voltage settings - 0x4 - 1.1 volt\n"
        "#cldo_pwm=0x4\n"
        "\n"
        "#VCO freq 326.4MHz\n"
        "spurconfig=0x3 \n"
        "\n"
        "# Improved Bluetooth coexistence parameters from Cypress\n"
        "btc_mode=1\n"
        "btc_params8=0x4e20\n"
        "btc_params1=0x7530\n";
    // /lib/modules/
    create_directories(root / "lib/modules");
    for (recursive_directory_iterator it(fwRoot / "modules"); it != recursive_directory_iterator(); ++it) {
        copy(*it, root / "lib" / it->path().native().substr(fwRoot.native().size()));
    }
}
