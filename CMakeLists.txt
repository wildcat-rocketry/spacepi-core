cmake_minimum_required(VERSION 3.0)
project(spacepi-targetlib-linux)
include(core/cmake/SpacePi.cmake)

spacepi_once(TARGETLIB_LINUX_REPO)

spacepi_dependencies(
    BLKID_LIB
    BOOST_ASIO
    BOOST_FILESYSTEM
    BOOST_PROCESS
    BOOST_SYSTEM
    DEBOOTSTRAP
    GIT_LIB
    KPARTX
    MKFS
    SFDISK
    USERADD
)

spacepi_dependencies(
    OPTIONAL
    SU
    SUDO
)

get_target_property(SFDISK_EXECUTABLE SFDISK IMPORTED_LOCATION)
get_target_property(KPARTX_EXECUTABLE KPARTX IMPORTED_LOCATION)
get_target_property(MKFS_EXECUTABLE MKFS IMPORTED_LOCATION)
get_target_property(DEBOOTSTRAP_EXECUTABLE DEBOOTSTRAP IMPORTED_LOCATION)
get_target_property(USERADD_EXECUTABLE USERADD IMPORTED_LOCATION)
if (TARGET SUDO)
    get_target_property(SUDO_EXECUTABLE SUDO IMPORTED_LOCATION)
endif()
if (TARGET SU)
    get_target_property(SU_EXECUTABLE SU IMPORTED_LOCATION)
endif()

configure_file(include/spacepi/liblinux/Config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/include/spacepi/liblinux/Config.hpp @ONLY)

spacepi_aux_library(
    spacepi-targetlib-linux SHARED
    SOURCES
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
        src/steps/EnsureRootStep.cpp
        src/steps/FormatPartitionsStep.cpp
        src/steps/InitStep.cpp
        src/steps/InstallBaseSystemStep.cpp
        src/steps/InstallSpacePiStep.cpp
        src/steps/InstallSystemFilesStep.cpp
        src/steps/MountPartitionsStep.cpp
        src/SystemCaller.cpp
        src/UniqueChroot.cpp
        src/UniqueEID.cpp
        src/UniqueLoopDevice.cpp
        src/UniqueMount.cpp
        src/UniqueProcess.cpp
        src/UniqueTempDir.cpp
    CONFIG_FILE include/spacepi/liblinux/Config.hpp
    INCLUDE include
    PRECOMPILE_FROM spacepi
    LINK
        spacepi
        BLKID_LIB
        BOOST_ASIO
        BOOST_FILESYSTEM
        BOOST_PROCESS
        BOOST_SYSTEM
        GIT_LIB
)

add_subdirectory(extension)
add_subdirectory(setup-deploy-key)
add_subdirectory(spacepictl)
