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

set(LLVM_ENABLE_PROJECTS "clang" CACHE STRING ".")
set(LLVM_TARGETS_TO_BUILD "WebAssembly" CACHE STRING ".")
set(LLVM_BUILD_TOOLS OFF CACHE STRING ".")
set(LLVM_ENABLE_BACKTRACES OFF CACHE STRING ".")
set(LLVM_ENABLE_BINDINGS OFF CACHE STRING ".")
set(LLVM_ENABLE_LIBPFM OFF CACHE STRING ".")
set(LLVM_ENABLE_ZLIB OFF CACHE STRING ".")
set(LLVM_EXTERNAL_PROJECTS "SpacePi.Format.Natives" CACHE STRING ".")
set(LLVM_EXTERNAL_SPACEPI.FORMAT.NATIVES_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/format/Natives" CACHE PATH ".")
set(LLVM_INCLUDE_BENCHMARKS OFF CACHE STRING ".")
set(LLVM_INCLUDE_EXAMPLES OFF CACHE STRING ".")
set(LLVM_INCLUDE_TESTS OFF CACHE STRING ".")
add_subdirectory(llvm/llvm llvm EXCLUDE_FROM_ALL)

add_subdirectory(dashboard)
add_subdirectory(docs)
add_subdirectory(dtc)
add_subdirectory(format)
add_subdirectory(protoc)
add_subdirectory(test)
