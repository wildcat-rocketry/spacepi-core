cmake_minimum_required(VERSION 3.0)
project(spacepi-target-rpi)
include(core/cmake/TargetRepo.cmake)

add_subdirectory(core)
add_subdirectory(spacepi-targetlib-linux)

spacepi_dependencies(
    BOOST_FILESYSTEM
    GIT_EXE
    QEMU_USER_STATIC arm
    DTC
)

get_target_property(GIT_EXECUTABLE GIT_EXE IMPORTED_LOCATION)
get_target_property(QEMU_ARM_STATIC_EXECUTABLE QEMU_USER_STATIC_arm IMPORTED_LOCATION)

set(GPIO_LINE_NAMES_DTBO_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/gpio-line-names.dtbo")

add_custom_command(
    OUTPUT "${GPIO_LINE_NAMES_DTBO_OUTPUT}"
    COMMAND DTC -@ -Hepapr -I dts -O dtb -o "${GPIO_LINE_NAMES_DTBO_OUTPUT}" "${CMAKE_CURRENT_SOURCE_DIR}/src/gpio-line-names.dts"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/gpio-line-names.dts"
    COMMENT "Running device tree compiler on gpio-line-names.dts"
    VERBATIM
)
add_custom_target(
    gpio-line-names-dtbo-dtc
    DEPENDS "${GPIO_LINE_NAMES_DTBO_OUTPUT}"
)
add_library(gpio-line-names-dtbo INTERFACE)
add_dependencies(gpio-line-names-dtbo gpio-line-names-dtbo-dtc)

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
        gpio-line-names-dtbo
)
