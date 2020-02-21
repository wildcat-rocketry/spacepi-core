#!/bin/bash
set -e

(
    mount -t proc none /proc
    mount /dev/mmcblk0p1 /boot
    mount -o remount,rw /

    mount /dev/mmcblk0p4 /var
    rngd -r /dev/hwrng
    dpkg-reconfigure openssh-server
    umount /var

    growpart /dev/mmcblk0 4
    fsck -f /dev/mmcblk0p4
    resize2fs /dev/mmcblk0p4

    sed -e "s|init=/spacepi/setup.sh||" -i /boot/cmdline.txt

    sync
    mount -o remount,ro /
    umount /boot
    echo b > /proc/sysrq-trigger
) || (
    while true; do
        sleep 1
    done
)
