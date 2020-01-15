cmake_minimum_required(VERSION 3.0)
project(spacepi-os VERSION 1.0)

add_custom_target(os
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/armel/spacepi.txt
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/armhf/spacepi.txt
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/arm64/spacepi.txt
)

add_custom_target(os_clean
    COMMAND rm -rf armel
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

set(OS_BASE_PACKAGES systemd,systemd-sysv,cmake,gcc,raspi3-firmware)
set(OS_DIST_NAME stable)
set(OS_REPO_URL http://deb.debian.org/debian)

configure_file(build.sh .)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/armel/spacepi.txt
    COMMAND ./build.sh armel qemu-arm-static
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/armhf/spacepi.txt
    COMMAND ./build.sh armhf qemu-arm-static
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/arm64/spacepi.txt
    COMMAND ./build.sh arm64 qemu-arm-static
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
