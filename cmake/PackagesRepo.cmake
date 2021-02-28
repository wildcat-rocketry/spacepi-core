include(${CMAKE_CURRENT_LIST_DIR}/components/SpacePi.cmake)

spacepi_once(PACKAGES_CMAKE_REPO)

add_custom_target(
    installable
    DEPENDS
        extensions
        libraries
        modules
        programs
)

spacepi_once(CORE_CMAKE_REPO)

add_custom_target(
    default ALL
    DEPENDS
        extensions
        libraries
        modules
        programs
        targets
)
