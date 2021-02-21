cmake_minimum_required(VERSION 3.0)
project(spacepi-target-rpi)
include(core/cmake/TargetRepo.cmake)

add_subdirectory(spacepi-targetlib-linux)

find_program(QEMU_ARM_STATIC_EXECUTABLE NAMES qemu-arm-static REQUIRED)

configure_file(include/spacepi/target/Config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/include/spacepi/target/Config.hpp @ONLY)

spacepi_target(
    src/main.cpp
    src/OSBuilder.cpp
)

spacepi_target_link_libraries(PUBLIC
    spacepi-targetlib-linux
)
spacepi_target_include_directories(PUBLIC
    include
    "${CMAKE_CURRENT_BINARY_DIR}/include"
)

add_subdirectory(install-spacepi-boardsupport)
add_subdirectory(os)
