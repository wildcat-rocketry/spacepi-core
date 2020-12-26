cmake_minimum_required(VERSION 3.0)
project(spacepi-target-rpi)
include(core/cmake/TargetRepo.cmake)

add_subdirectory(spacepi-targetlib-linux)

spacepi_target(
    src/main.cpp
    src/OSBuilder.cpp
)

spacepi_target_link_libraries(PUBLIC
    spacepi-targetlib-linux
)
spacepi_target_include_directories(PUBLIC include)

add_subdirectory(install-spacepi-boardsupport)
add_subdirectory(os)
