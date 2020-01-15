#!/bin/bash
set -e

if [ $# -ne 2 ]; then
    echo "Usage: $0 [architecture] [qemu command]" >&2
    exit 1
fi

if [ -d "$1" ]; then
    rm -rf "$1"
fi

debootstrap --foreign --arch="$1" --include=${OS_BASE_PACKAGES} --components main,contrib,non-free ${OS_DIST_NAME} "$1" ${OS_REPO_URL}
qemu_path="$(dirname "$(which "$2")")"
mkdir -p "$1$qemu_path"
cp "$qemu_path/$2" "$1$qemu_path"
chroot "$1" /debootstrap/debootstrap --second-stage

touch "$1/spacepi.txt"
