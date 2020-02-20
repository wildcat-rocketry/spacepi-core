#!/bin/bash
set -e

if [ $# -ne 2 ]; then
    echo "Usage: $0 [architecture] [qemu command]" >&2
    exit 1
fi

if [ $(id -u) -ne 0 ]; then
    echo "Must run script as root" >&2
    exit 1
fi

if [ -d "$1" ]; then
    umount "$1/boot" || true
    umount "$1/mnt" || true
    umount "$1/spacepi/bin" || true
    umount "$1/spacepi/build" || true
    umount "$1/spacepi/code" || true
    umount "$1/tmp" || true
    umount "$1/var" || true
    umount "$1" || true
    rmdir "$1"
fi
if [ -f "$1.iso~" ]; then
    kpartx -d "$1.iso~" || true
    rm -f "$1.iso~"
fi

dd if=/dev/zero of="$1.iso~" bs=1 count=0 seek=3G
sfdisk "$1.iso~" < "@CMAKE_CURRENT_SOURCE_DIR@/partitions.sfdisk"
dev="$(kpartx -asv "$1.iso~" | grep -E "^add map loop[0-9]+p1 " | sed -e "s|^add map \(loop[0-9]*\)p1 .*$|\1|")"
mkfs.fat -F32 /dev/mapper/${dev}p1
for part in p3 p4 p5 p6 p7; do
    mkfs.ext4 /dev/mapper/${dev}$part
done

if [ ! -f "@CMAKE_CURRENT_SOURCE_DIR@/deploy-key/id_rsa" ]; then
    ssh-keygen -t rsa -b 4096 -N "" -C "spacepi-deploy-$(hostname)" -f "@CMAKE_CURRENT_SOURCE_DIR@/deploy-key/id_rsa"
fi
if [ ! -f "@CMAKE_CURRENT_SOURCE_DIR@/deploy-key/id_rsa.pub" ]; then
    ssh-keygen -y -f "@CMAKE_CURRENT_SOURCE_DIR@/deploy-key/id_rsa" > "@CMAKE_CURRENT_SOURCE_DIR@/deploy-key/id_rsa.pub"
    ssh-keygen -c -C "spacepi-deploy-$(hostname)" -f "@CMAKE_CURRENT_SOURCE_DIR@/deploy-key/id_rsa"
fi

mkdir -p "$1"
mount /dev/mapper/${dev}p3 "$1"

mkdir -p cache
# Sometimes it "fails" to download a package, but running it again fixes the problem
download_os_arch="$1"
download_os() {
    debootstrap --foreign --arch="$download_os_arch" --include=@OS_BASE_PACKAGES@ --components main,contrib,non-free --cache-dir="$(readlink -f cache)" @OS_DIST_NAME@ "$download_os_arch" @OS_REPO_URL@
}
download_os || download_os || download_os
qemu_path="$(dirname "$(which "$2")")"
mkdir -p "$1$qemu_path"
install -o root -g root -m 0755 "$qemu_path/$2" "$1$qemu_path"
chroot "$1" /debootstrap/debootstrap --second-stage

mkdir -p "$1/boot" "$1/spacepi/code" "$1/spacepi/build" "$1/spacepi/bin" "$1/var/local/home" "$1/lib/modules" "$1/var/log/journal"
for file in environment fstab hostname resolv.conf; do
    install -o root -g root -m 0644 "@CMAKE_CURRENT_SOURCE_DIR@/$file" "$1/etc"
done
install -o root -g root -m 0644 "@CMAKE_CURRENT_SOURCE_DIR@/spacepi-ld.conf" "$1/etc/ld.so.conf.d"
install -o root -g root -m 0600 "@CMAKE_CURRENT_SOURCE_DIR@/ethernet.nmconnection" "$1/etc/NetworkManager/system-connections"
ln -sf bash "$1/bin/sh"
patch -d "$1/etc/" < "@CMAKE_CURRENT_SOURCE_DIR@/sudoers.patch"
patch -d "$1/usr/bin/" < "@CMAKE_CURRENT_SOURCE_DIR@/growpart.patch"
patch -d "$1/etc/skel/" < "@CMAKE_CURRENT_SOURCE_DIR@/bashrc.patch"
patch -d "$1/lib/systemd/system" < "@CMAKE_CURRENT_SOURCE_DIR@/getty.patch"
install -o root -g root -m 0744 "@CMAKE_CURRENT_SOURCE_DIR@/setup.sh" "@CMAKE_CURRENT_SOURCE_DIR@/wifi.sh" "@CMAKE_CURRENT_SOURCE_DIR@/ld-setup.sh" "$1/spacepi"
install -o root -g root -m 0644 "@CMAKE_CURRENT_SOURCE_DIR@/wifi-preinit.service" "@CMAKE_CURRENT_SOURCE_DIR@/wifi-init.service" "@CMAKE_CURRENT_SOURCE_DIR@/spacepi-ld-setup.service"  "$1/etc/systemd/system"
install -o root -g root -m 0644 "@CMAKE_CURRENT_SOURCE_DIR@/brcmfmac43430-sdio.txt" "$1/lib/firmware/brcm"
chroot "$1" systemctl enable wifi-preinit.service
chroot "$1" systemctl enable wifi-init.service
chroot "$1" systemctl enable spacepi-ld-setup.service
chroot "$1" systemctl mask wpa_supplicant.service
for user_file in "@CMAKE_CURRENT_SOURCE_DIR@/users/"*; do
    user="$(basename "$user_file")"
    chroot "$1" useradd -g sudo -m -N "$user"
    mkdir -p "$1/home/$user/.ssh"
    tail -n +3 "$user_file" > "$1/home/$user/.ssh/authorized_keys"
    chmod 0644 "$1/home/$user/.ssh/authorized_keys"
    install -m 0400 "@CMAKE_CURRENT_SOURCE_DIR@/deploy-key/id_rsa" "@CMAKE_CURRENT_SOURCE_DIR@/deploy-key/id_rsa.pub" "$1/home/$user/.ssh"
    chroot "$1" su "$user" -c "git config --global user.name \"$(head -n 1 "$user_file")\""
    chroot "$1" su "$user" -c "git config --global user.email \"$(head -n 2 "$user_file" | tail -n 1)\""
    chroot "$1" chown -R "$user:sudo" "/home/$user"
done

# Need to patch after adding users to keep comment in place
patch -d "$1/etc/" < "@CMAKE_CURRENT_SOURCE_DIR@/shadow.patch"

rm -f "$1/etc/ssh/"ssh_host_*

if [ -d firmware ]; then
    git -C firmware fetch --depth=1
    git -C firmware checkout origin/master
    git -C firmware branch -D master
    git -C firmware checkout -b master
else
    git clone --depth=1 "https://github.com/raspberrypi/firmware.git"
fi
if [ -d paho.mqtt.c ]; then
    git -C paho.mqtt.c pull
else
    git clone "https://github.com/eclipse/paho.mqtt.c.git"
fi

cp -R firmware/modules/* "$1/lib/modules"
mount /dev/mapper/${dev}p1 "$1/boot"
cp -R firmware/boot/* "@CMAKE_CURRENT_SOURCE_DIR@/cmdline.txt" "@CMAKE_CURRENT_SOURCE_DIR@/config.txt" "$1/boot"
mount /dev/mapper/${dev}p5 "$1/spacepi/code"
chroot "$1" git -C /spacepi/code init
chroot "$1" git -C /spacepi/code remote add origin "git@gitlab.com:ffaero/tools/space-pi.git"
mount /dev/mapper/${dev}p6 "$1/spacepi/build"
mount /dev/mapper/${dev}p7 "$1/spacepi/bin"
chroot "$1" chown -R root:sudo /spacepi
mount /dev/mapper/${dev}p4 "$1/mnt"
cp -R --preserve=all "$1/var/"* "$1/mnt"
umount "$1/mnt"
rm -rf "$1/var/"*
mount /dev/mapper/${dev}p4 "$1/var"
cp -R --preserve=all "$1/home/"* "$1/var/local/home"
rm -rf "$1/home/"*
install -o root -g root -m 0644 "@CMAKE_CURRENT_SOURCE_DIR@/adjtime" "$1/var/local"
ln -sf /var/local/adjtime "$1/etc/adjtime"
patch -d "$1/etc/init.d" < "@CMAKE_CURRENT_SOURCE_DIR@/hwclock.patch"
ln -sf /proc/self/mounts "$1/etc/mtab"
touch "$1/var/local/wifi"
chown root:root "$1/var/local/wifi"
ln -sf /var/local/wifi "$1/etc/network/interfaces.d/wifi"
mount -o bind paho.mqtt.c "$1/mnt"
mount -t tmpfs none "$1/tmp"
chroot "$1" cmake -S /mnt -B /tmp
chroot "$1" make -C /tmp install
umount "$1/tmp"
umount "$1/mnt"
umount "$1/var"
umount "$1/spacepi/build"
umount "$1/spacepi/bin"
umount "$1/spacepi/code"
umount "$1/boot"
umount "$1"

rmdir "$1"
kpartx -d "$1.iso~"
sync
mv -f "$1.iso~" "$1.iso"
