add_custom_target(targets)
add_dependencies(full targets)

# spacepi_target(
#     SOURCES <source1> [<source2> ...]
#     [CONFIG_FILE <file>]
#     [INCLUDE <dir1> [<dir2> ...]]
#     [PRECOMPILE <file1> [<file2> ...]]
#     [LINK <lib1> [<lib2> ...]]
#     [EXAMPLE]
# )
function (spacepi_target)
    cmake_parse_arguments(SPACEPI_TAR "EXAMPLE" "CONFIG_FILE" "SOURCES;INCLUDE;PRECOMPILE;LINK" ${ARGN})

    spacepi_directory_name(name "target")

    set(args
        "${name}"
        SOURCES ${SPACEPI_TAR_SOURCES}
        PRECOMPILE_FROM spacepi_pch
        LINK ${SPACEPI_TAR_LINK} spacepi
        NO_INSTALL
        ALL_TARGET targets
    )

    if (SPACEPI_TAR_EXAMPLE)
        list(APPEND args EXAMPLE)
    endif()

    if (SPACEPI_TAR_CONFIG_FILE)
        list(APPEND args CONFIG_FILE "${SPACEPI_TAR_CONFIG_FILE}")
    endif()

    if (SPACEPI_TAR_INCLUDE)
        list(APPEND args INCLUDE ${SPACEPI_TAR_INCLUDE})
    endif()

    if (SPACEPI_TAR_PRECOMPILE)
        list(APPEND args PRECOMPILE ${SPACEPI_TAR_PRECOMPILE})
    endif()

    spacepi_aux_program(${args})
endfunction()
