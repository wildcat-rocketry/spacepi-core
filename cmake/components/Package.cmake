define_property(TARGET PROPERTY SPACEPI_PKG_LIST BRIEF_DOCS "." FULL_DOCS ".")

add_custom_target(packages)
set_target_properties(packages PROPERTIES SPACEPI_PKG_LIST "")
add_dependencies(full packages)

# spacepi_package(
#     <file>
#     [BUILD <name> <target> <image>]
# )
function (spacepi_package SPACEPI_PKG_FILE)
    cmake_parse_arguments(SPACEPI_PKG "" "" "BUILD" ${ARGN})

    get_target_property(pkgList packages SPACEPI_PKG_LIST)
    list(APPEND pkgList "${CMAKE_CURRENT_SOURCE_DIR}/${SPACEPI_PKG_FILE}")
    set_target_properties(packages PROPERTIES SPACEPI_PKG_LIST "${pkgList}")

    if (SPACEPI_PKG_BUILD)
        list(GET SPACEPI_PKG_BUILD "0" SPACEPI_PKG_NAME)
        list(GET SPACEPI_PKG_BUILD "1" SPACEPI_PKG_TARGET)
        list(GET SPACEPI_PKG_BUILD "2" SPACEPI_PKG_IMAGE)

        spacepi_directory_name(name "target" DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${SPACEPI_PKG_TARGET}")

        add_custom_target(
            "${SPACEPI_PKG_NAME}"
            COMMAND "${name}" --config-file "${CMAKE_CURRENT_SOURCE_DIR}/${SPACEPI_PKG_FILE}" --out "${CMAKE_CURRENT_BINARY_DIR}/${SPACEPI_PKG_IMAGE}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            DEPENDS "${name}"
            COMMENT "Generating package from ${SPACEPI_PKG_NAME}"
            VERBATIM
        )

        add_dependencies(packages "${SPACEPI_PKG_NAME}")
    endif()
endfunction()
