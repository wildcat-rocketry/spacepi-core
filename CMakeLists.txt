cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0091 NEW)
project(spacepi-core)
include(cmake/SpacePiSetup.cmake)

include(SpacePiOnce)
spacepi_once(SPACEPI_CORE)

set(core_include_dirs
    include
    platform
    platform/spacepi/platform/host
    "${CMAKE_CURRENT_BINARY_DIR}/include"
)

set(core_defines
    SPACEPI_HAS_BUILD_CONFIG
    SPACEPI_HAS_PLATFORM_IMPL
)

add_library(spacepi-pch STATIC src/PCH.cpp)
target_include_directories(spacepi-pch PUBLIC ${core_include_dirs})
target_compile_definitions(spacepi-pch PUBLIC ${core_defines})
target_precompile_headers(spacepi-pch PUBLIC <SpacePi.hpp>)
configure_file(include/spacepi/BuildConfig.in.hpp "${CMAKE_CURRENT_BINARY_DIR}/include/spacepi/BuildConfig.hpp")

add_library(spacepi-platform STATIC src/Platform.cpp)
target_include_directories(spacepi-platform PUBLIC ${core_include_dirs})
target_compile_definitions(spacepi-platform PUBLIC ${core_defines})
add_dependencies(spacepi-platform spacepi-pch)

add_subdirectory(dashboard)
add_subdirectory(docs)
add_subdirectory(dtc)
add_subdirectory(protoc)
add_subdirectory(test)
