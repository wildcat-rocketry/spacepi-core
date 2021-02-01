cmake_minimum_required(VERSION 3.0)
project(spacepi-targetlib-linux)
include(core/cmake/SpacePi.cmake)

add_library(spacepi-targetlib-linux
    src/steps/BuildSpacePiStep.cpp
    src/steps/CleanupStep.cpp
    src/steps/CloneGitRepoStep.cpp
    src/steps/FormatPartitionsStep.cpp
    src/steps/InstallBaseSystemStep.cpp
    src/steps/InstallSpacePiStep.cpp
    src/steps/InstallSystemFilesStep.cpp
    src/steps/MountPartitionsStep.cpp
    src/DefaultInstallationConfig.cpp
    src/DefaultInstallationPlan.cpp
    src/Image.cpp
    src/InstallationData.cpp
    src/InstallationPlan.cpp
    src/main.cpp
    src/Partition.cpp
    src/PartitionTable.cpp
    src/SharedMount.cpp
    src/SystemCaller.cpp
    src/UniqueMount.cpp
)

target_link_libraries(spacepi-targetlib-linux
    spacepi
)
target_include_directories(spacepi-targetlib-linux PUBLIC include)
target_precompile_headers(spacepi-targetlib-linux REUSE_FROM spacepi)

add_subdirectory(extension)
add_subdirectory(setup-deploy-key)
add_subdirectory(spacepictl)
