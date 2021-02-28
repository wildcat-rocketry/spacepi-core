add_custom_target(extensions)
add_dependencies(full extensions)

# spacepi_extension(
#     SOURCES <source1> [<source2> ...]
#     [CONFIG_FILE <file>]
#     [INCLUDE <dir1> [<dir2> ...]]
#     [PRECOMPILE <file1> [<file2> ...]]
#     [LINK <lib1> [<lib2> ...]]
#     [EXAMPLE]
# )
function (spacepi_extension)
    cmake_parse_arguments(SPACEPI_EXT "EXAMPLE" "CONFIG_FILE" "SOURCES;INCLUDE;PRECOMPILE;LINK" ${ARGN})

    spacepi_directory_name(name "ext")

    set(args
        "${name}" SHARED
        SOURCES ${SPACEPI_EXT_SOURCES}
        PRECOMPILE_FROM spacepi
        LINK ${SPACEPI_EXT_LINK} spacepi
        ALL_TARGET extensions
    )

    if (SPACEPI_EXT_EXAMPLE)
        list(APPEND args EXAMPLE)
    endif()

    if (SPACEPI_EXT_CONFIG_FILE)
        list(APPEND args CONFIG_FILE "${SPACEPI_EXT_CONFIG_FILE}")
    endif()

    if (SPACEPI_EXT_INCLUDE)
        list(APPEND args INCLUDE ${SPACEPI_EXT_INCLUDE})
    endif()

    if (SPACEPI_EXT_PRECOMPILE)
        list(APPEND args PRECOMPILE ${SPACEPI_EXT_PRECOMPILE})
    endif()

    spacepi_aux_library(${args})
endfunction()
