cmake_minimum_required(VERSION 3.0)
project(spacepi-target-docker)
include(core/cmake/TargetRepo.cmake)

add_subdirectory(spacepi-targetlib-linux)

spacepi_target_link_libraries(PUBLIC
    spacepi-targetlib-linux
)
