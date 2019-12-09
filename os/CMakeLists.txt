cmake_minimum_required(VERSION 3.0)
project(spacepi-os VERSION 1.0)

add_custom_target(os
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/rootfs/spacepi.txt
)

add_custom_target(os_clean
    COMMAND rm -rf ${CMAKE_CURRENT_BINARY_DIR}/rootfs
)

set(TARGET_ARCH armel CACHE STRING "The architecture to generate the OS files for")

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/rootfs/spacepi.txt
    COMMAND test -d "rootfs" && rm -rf rootfs || true
    COMMAND mkdir rootfs
    COMMAND debootstrap --foreign --arch=${TARGET_ARCH} stable rootfs http://deb.debian.org/debian
    COMMAND touch rootfs/spacepi.txt
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
