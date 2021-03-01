include(${CMAKE_CURRENT_LIST_DIR}/components/SpacePi.cmake)

spacepi_once(CORE_CMAKE_REPO)

option(BUILD_MINIMAL "Build only components which will be installed" OFF)

if (BUILD_MINIMAL)
    add_custom_target(
        default ALL
        DEPENDS
            installable
    )
else()
    add_custom_target(
        default ALL
        DEPENDS
            extensions
            libraries
            modules
            programs
            targets
    )
endif()
