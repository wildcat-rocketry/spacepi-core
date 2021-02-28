add_custom_target(programs)
add_dependencies(full programs)

# spacepi_aux_program(
#     <name>
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
function (spacepi_aux_program SPACEPI_PROG_NAME)
    cmake_parse_arguments(SPACEPI_PROG "NO_INSTALL;EXAMPLE" "CONFIG_FILE;ALL_TARGET" "SOURCES;INCLUDE;PRECOMPILE;PRECOMPILE_FROM;LINK" ${ARGN})

    add_executable(
        "${SPACEPI_PROG_NAME}"
        EXCLUDE_FROM_ALL
        ${SPACEPI_PROG_SOURCES}
    )
    
    set(args "${SPACEPI_PROG_NAME}" DEFAULT_ALL_TARGET programs)

    if (SPACEPI_PROG_EXAMPLE)
        list(APPEND args EXAMPLE)
    endif()

    if (SPACEPI_PROG_CONFIG_FILE)
        list(APPEND args CONFIG_FILE "${SPACEPI_PROG_CONFIG_FILE}")
    endif()

    if (SPACEPI_PROG_ALL_TARGET)
        list(APPEND args ALL_TARGET "${SPACEPI_PROG_ALL_TARGET}")
    endif()

    if (SPACEPI_PROG_INCLUDE)
        list(APPEND args INCLUDE ${SPACEPI_PROG_INCLUDE})
    endif()

    if (SPACEPI_PROG_PRECOMPILE)
        list(APPEND args PRECOMPILE ${SPACEPI_PROG_PRECOMPILE})
    endif()

    if (SPACEPI_PROG_PRECOMPILE_FROM)
        list(APPEND args PRECOMPILE_FROM ${SPACEPI_PROG_PRECOMPILE_FROM})
    endif()

    if (SPACEPI_PROG_LINK)
        list(APPEND args LINK ${SPACEPI_PROG_LINK})
    endif()

    spacepi_aux_target(${args})

    if (NOT SPACEPI_PROG_NO_INSTALL AND NOT SPACEPI_PROG_EXAMPLE)
        install(TARGETS "${SPACEPI_PROG_NAME}" RUNTIME DESTINATION "bin")
    endif()
endfunction()
