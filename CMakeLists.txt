cmake_minimum_required(VERSION 3.0)
project(spacepi-targetlib-linux)
include(core/cmake/SpacePi.cmake)

find_package(Boost
    REQUIRED COMPONENTS
    # asio
    filesystem
    # process
    system
)

find_program(SFDISK_EXECUTABLE NAMES sfdisk REQUIRED)
find_program(KPARTX_EXECUTABLE NAMES kpartx REQUIRED)
find_program(MKFS_EXECUTABLE NAMES mkfs REQUIRED)
find_program(DEBOOTSTRAP_EXECUTABLE NAMES debootstrap REQUIRED)

configure_file(include/spacepi/liblinux/Config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/include/spacepi/liblinux/Config.hpp @ONLY)

add_library(spacepi-targetlib-linux
    src/DefaultInstallationConfig.cpp
    src/DefaultInstallationPlan.cpp
    src/Image.cpp
    src/InstallationData.cpp
    src/InstallationOptions.cpp
    src/InstallationPlan.cpp
    src/Partition.cpp
    src/PartitionTable.cpp
    src/SharedLoopDevice.cpp
    src/SharedMount.cpp
    src/SharedProcess.cpp
    src/SharedTempDir.cpp
    src/steps/BuildSpacePiStep.cpp
    src/steps/CleanupStep.cpp
    src/steps/CloneGitRepoStep.cpp
    src/steps/FormatPartitionsStep.cpp
    src/steps/InitStep.cpp
    src/steps/InstallBaseSystemStep.cpp
    src/steps/InstallSpacePiStep.cpp
    src/steps/InstallSystemFilesStep.cpp
    src/steps/MountPartitionsStep.cpp
    src/SystemCaller.cpp
    src/UniqueChroot.cpp
    src/UniqueLoopDevice.cpp
    src/UniqueMount.cpp
    src/UniqueProcess.cpp
    src/UniqueTempDir.cpp
)

target_link_libraries(spacepi-targetlib-linux
    Boost::filesystem
    Boost::system
    spacepi
)
target_include_directories(spacepi-targetlib-linux PUBLIC
    include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)
target_precompile_headers(spacepi-targetlib-linux REUSE_FROM spacepi)

add_subdirectory(extension)
add_subdirectory(setup-deploy-key)
add_subdirectory(spacepictl)
