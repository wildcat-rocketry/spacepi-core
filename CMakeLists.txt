cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0091 NEW)
project(spacepi-core)
include(cmake/SpacePiSetup.cmake)

include(SpacePiOnce)
spacepi_once(SPACEPI_CORE)

add_library(spacepi-pch STATIC src/PCH.cpp)
target_include_directories(spacepi-pch PUBLIC include)
target_precompile_headers(spacepi-pch PUBLIC <SpacePi.hpp>)

add_subdirectory(dashboard)
add_subdirectory(docs)
add_subdirectory(protoc)
add_subdirectory(test)
