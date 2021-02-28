cmake_minimum_required(VERSION 3.0)
project(spacepi-target-rpi)
include(core/cmake/TargetRepo.cmake)

add_subdirectory(spacepi-targetlib-linux)
add_subdirectory(install-spacepi-boardsupport)

spacepi_dependencies(
    BOOST_FILESYSTEM
    GIT_EXE
    QEMU_USER_STATIC arm
)

get_target_property(GIT_EXECUTABLE GIT_EXE IMPORTED_LOCATION)
get_target_property(QEMU_ARM_STATIC_EXECUTABLE QEMU_USER_STATIC_arm IMPORTED_LOCATION)

spacepi_target(
    SOURCES
        src/DownloadFirmwareStep.cpp
        src/Firmware.cpp
        src/InstallFirmwareStep.cpp
        src/main.cpp
        src/OSBuilder.cpp
    CONFIG_FILE include/spacepi/target/Config.hpp
    INCLUDE include
    LINK
        BOOST_FILESYSTEM
        spacepi-targetlib-linux
)
