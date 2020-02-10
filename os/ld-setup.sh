#!/bin/bash
set -e

mount -o remount,rw /
ldconfig
systemctl disable spacepi-ld-setup.service
reboot
