add_custom_target(libraries)
add_dependencies(full libraries)

# spacepi_aux_library(
#     <name>
#     <type>
#     SOURCES <source1> [<source2> ...]
#     [CONFIG_FILE <file>]
#     [INCLUDE <dir1> [<dir2> ...]]
#     [PRECOMPILE <file1> [<file2> ...]]
#     [PRECOMPILE_FROM <target1> [<target2> ...]]
#     [LINK <lib1> [<lib2> ...]]
#     [NO_INSTALL]
#     [EXAMPLE]
#     [ALL_TARGET <target>]
# )
function (spacepi_aux_library SPACEPI_LIB_NAME SPACEPI_LIB_TYPE)
    cmake_parse_arguments(SPACEPI_LIB "NO_INSTALL;EXAMPLE" "CONFIG_FILE;ALL_TARGET" "SOURCES;INCLUDE;PRECOMPILE;PRECOMPILE_FROM;LINK" ${ARGN})

    add_library(
        "${SPACEPI_LIB_NAME}"
        "${SPACEPI_LIB_TYPE}"
        EXCLUDE_FROM_ALL
        ${SPACEPI_LIB_SOURCES}
    )

    set(args "${SPACEPI_LIB_NAME}" DEFAULT_ALL_TARGET libraries)

    if (SPACEPI_LIB_EXAMPLE)
        list(APPEND args EXAMPLE)
    endif()

    if (SPACEPI_LIB_CONFIG_FILE)
        list(APPEND args CONFIG_FILE "${SPACEPI_LIB_CONFIG_FILE}")
    endif()

    if (SPACEPI_LIB_ALL_TARGET)
        list(APPEND args ALL_TARGET "${SPACEPI_LIB_ALL_TARGET}")
    endif()

    if (SPACEPI_LIB_INCLUDE)
        list(APPEND args INCLUDE ${SPACEPI_LIB_INCLUDE})
    endif()

    if (SPACEPI_LIB_PRECOMPILE)
        list(APPEND args PRECOMPILE ${SPACEPI_LIB_PRECOMPILE})
    endif()

    if (SPACEPI_LIB_PRECOMPILE_FROM)
        list(APPEND args PRECOMPILE_FROM ${SPACEPI_LIB_PRECOMPILE_FROM})
    endif()

    if (SPACEPI_LIB_LINK)
        list(APPEND args LINK ${SPACEPI_LIB_LINK})
    endif()

    spacepi_aux_target(${args})

    if (NOT SPACEPI_LIB_NO_INSTALL)
        install(TARGETS "${SPACEPI_LIB_NAME}" RUNTIME DESTINATION "lib")
    endif()
endfunction()
