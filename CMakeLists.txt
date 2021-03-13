cmake_minimum_required(VERSION 3.0)
project(spacepi-targetlib-linux)
include(core/cmake/TargetRepo.cmake)
include(CheckCXXSourceCompiles)

add_subdirectory(core)

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
    SSH
)

spacepi_dependencies(
    OPTIONAL
    SU
    SUDO
)

set(CMAKE_REQUIRED_LIBRARIES GIT_LIB)
check_cxx_source_compiles("
    #include <git2/version.h>

    #if LIBGIT2_VER_MAJOR <= 0 && LIBGIT2_VER_MINOR < 23
    #error outdated libgit2 version!
    #endif

    int main() {
        return 0;
    }
" HAVE_NEW_LIBGIT2)
if (NOT HAVE_NEW_LIBGIT2)
    message(SEND_ERROR "Libgit2 version >= 0.23 required but not found.")
endif()

get_target_property(SFDISK_EXECUTABLE SFDISK IMPORTED_LOCATION)
get_target_property(KPARTX_EXECUTABLE KPARTX IMPORTED_LOCATION)
get_target_property(MKFS_EXECUTABLE MKFS IMPORTED_LOCATION)
get_target_property(DEBOOTSTRAP_EXECUTABLE DEBOOTSTRAP IMPORTED_LOCATION)
get_target_property(USERADD_EXECUTABLE USERADD IMPORTED_LOCATION)
get_target_property(SSH_EXECUTABLE SSH IMPORTED_LOCATION)
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
        src/DefaultConnectPlan.cpp
        src/DefaultInstallationConfig.cpp
        src/DefaultInstallationPlan.cpp
        src/Image.cpp
        src/InstallationData.cpp
        src/InstallationOptions.cpp
        src/InstallationPlan.cpp
        src/Installer.cpp
        src/Partition.cpp
        src/PartitionTable.cpp
        src/SharedLoopDevice.cpp
        src/SharedMount.cpp
        src/SharedProcess.cpp
        src/SharedTempDir.cpp
        src/steps/BuildSpacePiStep.cpp
        src/steps/CleanupStep.cpp
        src/steps/CloneGitRepoStep.cpp
        src/steps/CloneImageStep.cpp
        src/steps/ConnectStep.cpp
        src/steps/EnsureRootStep.cpp
        src/steps/FormatPartitionsStep.cpp
        src/steps/InitStep.cpp
        src/steps/InstallBaseSystemStep.cpp
        src/steps/InstallSpacePiStep.cpp
        src/steps/InstallSystemFilesStep.cpp
        src/steps/MountPartitionsStep.cpp
        src/steps/PrepareNFSStep.cpp
        src/steps/SSHTunnelStep.cpp
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
    NO_INSTALL
)

add_subdirectory(extension)
add_subdirectory(setup-deploy-key)
add_subdirectory(spacepictl)
