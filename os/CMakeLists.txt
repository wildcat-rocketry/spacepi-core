cmake_minimum_required(VERSION 3.0)
project(spacepi-os VERSION 1.0)

add_custom_target(os
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/armel.iso
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/armhf.iso
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/arm64.iso
)

add_custom_target(os_clean
    COMMAND rm -rf cache
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS os_reset
)

add_custom_target(os_reset
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/clean.sh armel
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/clean.sh armhf
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/clean.sh arm64
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

set(OS_BASE_PACKAGES build-essential,cloud-utils,cmake,firmware-brcm80211,gcc,git,g++,libgps-dev,make,mosquitto,network-manager,openssh-server,psmisc,rng-tools,sudo,systemd,systemd-sysv,wireless-tools,gdb,gpsd)

set(OS_DIST_NAME stable)
set(OS_REPO_URL http://deb.debian.org/debian)

configure_file(build.sh . @ONLY)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/armel.iso
    COMMAND ./build.sh armel qemu-arm-static
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/armhf.iso
    COMMAND ./build.sh armhf qemu-arm-static
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/arm64.iso
    COMMAND ./build.sh arm64 qemu-arm-static
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
