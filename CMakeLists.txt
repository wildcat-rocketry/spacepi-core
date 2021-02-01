cmake_minimum_required(VERSION 3.0)
project(spacepi-target-docker)
include(core/cmake/TargetRepo.cmake)

add_executable(spacepi-bootstrap
    src/main.cpp
)
install(TARGETS spacepi-bootstrap RUNTIME DESTINATION sbin)

add_subdirectory(spacepi-targetlib-linux)
