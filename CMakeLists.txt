cmake_minimum_required(VERSION 3.0)
project(spacepi-target-rpi)
include(core/cmake/TargetRepo.cmake)

spacepi_target(
    src/main.cpp
    src/OSBuilder.cpp
)

spacepi_target_include_directories(PUBLIC include)

add_subdirectory(setup-deploy-key)
