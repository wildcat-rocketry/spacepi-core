set(version 2.1.1)
set(build_compat 2.1.0)

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
    get_filename_component(parent2 "${parent}" DIRECTORY)

    message(FATAL_ERROR
        "SpacePi Core version mismatch!\n"
        "Loaded build script version: ${version} (from ${SPACEPI_CORE_SOURCE_DIR})\n"
        "Conflicting build script version: ${coreVersion} (from ${parent2})"
    )
endif()

if (SPACEPI_CORE_CACHE_VERSION VERSION_LESS build_compat)
    message(FATAL_ERROR
        "SpacePi build directory is too old (${SPACEPI_CORE_CACHE_VERSION}) to be used by current build scripts (${version})\n"
        "Please delete the build folder and re-create it to continue"
    )
endif()
