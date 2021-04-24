# Current version information
set(version 3.0.0)
set(build_compat 2.1.0)

# Enable testing
enable_testing()

# Ensure SPACEPI_CORE_DIR is set
get_property(hasProp GLOBAL PROPERTY SPACEPI_CORE_DIR DEFINED)
if (NOT hasProp)
    define_property(GLOBAL PROPERTY SPACEPI_CORE_DIR BRIEF_DOCS "." FULL_DOCS ".")
    get_filename_component(parent "${CMAKE_CURRENT_LIST_DIR}" DIRECTORY)
    set_property(GLOBAL PROPERTY SPACEPI_CORE_DIR "${parent}")
endif()

# Add /cmake/ to CMAKE_MODULE_PATH path
get_property(spacePiCore GLOBAL PROPERTY SPACEPI_CORE_DIR)
list(FIND CMAKE_MODULE_PATH "${spacePiCore}/cmake" res)
if (res LESS 0)
    list(APPEND CMAKE_MODULE_PATH "${spacePiCore}/cmake")
endif()

# Check version support
get_property(hasProp GLOBAL PROPERTY SPACEPI_CORE_VERSION DEFINED)
if (NOT hasProp)
    define_property(GLOBAL PROPERTY SPACEPI_CORE_VERSION BRIEF_DOCS "." FULL_DOCS ".")
    set_property(GLOBAL PROPERTY SPACEPI_CORE_VERSION "${version}")
endif()
if (NOT SPACEPI_CORE_CACHE_VERSION)
    set(SPACEPI_CORE_CACHE_VERSION "${version}" CACHE INTERNAL "")
endif()
get_property(coreVersion GLOBAL PROPERTY SPACEPI_CORE_VERSION)
if (NOT version VERSION_EQUAL coreVersion)
    get_filename_component(parent "${CMAKE_CURRENT_LIST_DIR}" DIRECTORY)
    message(FATAL_ERROR
        "SpacePi Core version mismatch!\n"
        "Loaded build script version: ${coreVersion} (from ${spacePiCore})\n"
        "Conflicting build script version: ${version} (from ${parent})"
    )
endif()
if (SPACEPI_CORE_CACHE_VERSION VERSION_LESS build_compat)
    message(FATAL_ERROR
        "SpacePi build directory is too old (${SPACEPI_CORE_CACHE_VERSION}) to be used by current build scripts (${version})\n"
        "Please delete the build folder and re-create it to continue"
    )
endif()

include(SpacePiCompat)

# Include platform code
include(SpacePiPlatform)
spacepi_has_platform(hasPlat)
if (NOT hasPlat)
    if (SPACEPI_PLATFORM_ID)
        add_subdirectory("${CMAKE_SOURCE_DIR}/${SPACEPI_PLATFORM_ID}" "${CMAKE_BINARY_DIR}/_platform")
    else()
        add_subdirectory("${spacePiCore}/platform-std" "${CMAKE_BINARY_DIR}/_platform")
    endif()
endif()
