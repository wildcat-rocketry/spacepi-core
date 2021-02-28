# spacepi_directory_name(
#     <out target name>
#     <target type>
#     [DIRECTORY <directory>]
# )
function (spacepi_directory_name SPACEPI_DIR_VAR SPACEPI_DIR_TYPE)
    cmake_parse_arguments(SPACEPI_DIR "" "DIRECTORY" "" ${ARGN})
    if (NOT SPACEPI_DIR_DIRECTORY)
        set(SPACEPI_DIR_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    endif()

    file(RELATIVE_PATH relDir "${CMAKE_SOURCE_DIR}" "${SPACEPI_DIR_DIRECTORY}")
    string(REGEX REPLACE "[^-.0-9A-Za-z]" "_" compName "${relDir}")
    if (compName STREQUAL "")
        set("${SPACEPI_DIR_VAR}" "spacepi-${SPACEPI_DIR_TYPE}" PARENT_SCOPE)
    else()
        set("${SPACEPI_DIR_VAR}" "spacepi-${SPACEPI_DIR_TYPE}-${compName}" PARENT_SCOPE)
    endif()
endfunction()
