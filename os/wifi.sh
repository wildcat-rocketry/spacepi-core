#!/bin/bash
set -e

if [ $# -ne 1 ]; then
    echo "Usage: $0 [target]" >&2
    exit 1
fi

ip=$(cat /etc/ip)

case $1 in
    preinit)
        truncate -s 0 /etc/network/interfaces.d/wifi
        for iface in $(iwconfig 2>&1 | grep -v "no wireless extension" | grep -E "^[^ ]" | cut -f 1 -d " "); do
            cat <<EOF >> /etc/network/interfaces.d/wifi
iface $iface inet static
    address $ip
    netmask 255.255.0.0
EOF
        done
        ;;
    init)
        for iface in $(grep -E "^iface " /etc/network/interfaces.d/wifi | cut -f 2 -d " "); do
            ip link set $iface down
            iwconfig $iface channel 1 essid space-pi mode ad-hoc
            ip link set $iface up
            ip addr add $ip/16 dev $iface
            # Not sure why I have to do this again but it makes it happy
            ip link set $iface down
            iwconfig $iface channel 1 essid space-pi mode ad-hoc
            ip link set $iface up
        done
        iwconfig
        ip addr
        ;;
    *)
        echo "Unknown target '$1'." >&2
        exit 1
        ;;
esac
