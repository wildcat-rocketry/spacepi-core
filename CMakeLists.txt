cmake_minimum_required(VERSION 3.0)
project(spacepi-targetlib-linux)
include(core/cmake/SpacePi.cmake)

add_library(spacepi-targetlib-linux
    src/main.cpp
)

target_link_libraries(spacepi-targetlib-linux
    spacepi
)
target_include_directories(spacepi-targetlib-linux PUBLIC include)
target_precompile_headers(spacepi-targetlib-linux REUSE_FROM spacepi)

add_subdirectory(extension)
add_subdirectory(setup-deploy-key)
add_subdirectory(spacepictl)
