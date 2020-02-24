#!/bin/bash
set -e

if [ $# -ne 1 ]; then
    echo "Usage: $0 [architecture]" >&2
    exit 1
fi

if [ $(id -u) -ne 0 ]; then
    echo "Must run script as root" >&2
    exit 1
fi

if [ -d "$1" ]; then
    umount "$1/boot" || true
    umount "$1/mnt" || true
    umount "$1/proc" || true
    umount "$1/sys" || true
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

rm -rf "$1.iso"
