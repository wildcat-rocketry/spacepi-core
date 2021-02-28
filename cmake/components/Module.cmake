add_custom_target(modules)
add_dependencies(full modules)

# spacepi_module(
#     SOURCES <source1> [<source2> ...]
#     [CONFIG_FILE <file>]
#     [INCLUDE <dir1> [<dir2> ...]]
#     [PRECOMPILE <file1> [<file2> ...]]
#     [LINK <lib1> [<lib2> ...]]
#     [EXAMPLE]
# )
function (spacepi_module)
    cmake_parse_arguments(SPACEPI_MOD "EXAMPLE" "CONFIG_FILE" "SOURCES;INCLUDE;PRECOMPILE;LINK" ${ARGN})

    spacepi_directory_name(name "mod")

    set(args
        "${name}"
        SOURCES ${SPACEPI_MOD_SOURCES}
        PRECOMPILE_FROM spacepi_pch
        LINK ${SPACEPI_MOD_LINK} spacepi
        ALL_TARGET modules
    )

    if (SPACEPI_MOD_EXAMPLE)
        list(APPEND args EXAMPLE)
    endif()

    if (SPACEPI_MOD_CONFIG_FILE)
        list(APPEND args CONFIG_FILE "${SPACEPI_MOD_CONFIG_FILE}")
    endif()

    if (SPACEPI_MOD_INCLUDE)
        list(APPEND args INCLUDE ${SPACEPI_MOD_INCLUDE})
    endif()

    if (SPACEPI_MOD_PRECOMPILE)
        list(APPEND args PRECOMPILE ${SPACEPI_MOD_PRECOMPILE})
    endif()

    spacepi_aux_program(${args})
endfunction()
