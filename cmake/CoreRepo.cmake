include(${CMAKE_CURRENT_LIST_DIR}/components/SpacePi.cmake)

spacepi_once(CORE_CMAKE_REPO)

add_custom_target(
    default ALL
    DEPENDS
        docs
        extensions
        libraries
        modules
        programs
        targets
)
