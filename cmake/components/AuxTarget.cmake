add_custom_target(examples)
add_dependencies(full examples)

# spacepi_aux_target(
#     <name>
#     [CONFIG_FILE <file>]
#     [INCLUDE <dir1> [<dir2> ...]]
#     [PRECOMPILE <file1> [<file2> ...]]
#     [PRECOMPILE_FROM <target1> [<target2> ...]]
#     [LINK <lib1> [<lib2> ...]]
#     [EXAMPLE]
#     [ALL_TARGET <target>]
#     [DEFAULT_ALL_TARGET <target>]
# )
function (spacepi_aux_target SPACEPI_AUXTAR_NAME)
    cmake_parse_arguments(SPACEPI_AUXTAR "EXAMPLE" "CONFIG_FILE" "INCLUDE;PRECOMPILE;PRECOMPILE_FROM;LINK;ALL_TARGET;DEFAULT_ALL_TARGET" ${ARGN})
    if (NOT SPACEPI_AUXTAR_ALL_TARGET)
        set(SPACEPI_AUXTAR_ALL_TARGET "${SPACEPI_AUXTAR_DEFAULT_ALL_TARGET}")
    endif()

    if (SPACEPI_AUXTAR_CONFIG_FILE)
        configure_file("${SPACEPI_AUXTAR_CONFIG_FILE}.in" "${CMAKE_CURRENT_BINARY_DIR}/${SPACEPI_AUXTAR_CONFIG_FILE}")
        set(found FALSE)
        foreach (dir IN LISTS SPACEPI_AUXTAR_INCLUDE)
            string(LENGTH "${dir}" dirLen)
            string(SUBSTRING "${SPACEPI_AUXTAR_CONFIG_FILE}" "0" "${dirLen}" includeDir)
            if (includeDir STREQUAL dir)
                target_include_directories("${SPACEPI_AUXTAR_NAME}" PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/${dir}")
                set(found TRUE)
            endif()
        endforeach()
        if (NOT found)
            target_include_directories("${SPACEPI_AUXTAR_NAME}" PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
        endif()
    endif()

    if (SPACEPI_AUXTAR_INCLUDE)
        target_include_directories("${SPACEPI_AUXTAR_NAME}" PUBLIC ${SPACEPI_AUXTAR_INCLUDE})
    endif()

    foreach (file IN LISTS SPACEPI_AUXTAR_PRECOMPILE)
        target_precompile_headers("${SPACEPI_AUXTAR_NAME}" PUBLIC "${file}")
    endforeach()

    foreach (target IN LISTS SPACEPI_AUXTAR_PRECOMPILE_FROM)
        target_precompile_headers("${SPACEPI_AUXTAR_NAME}" REUSE_FROM "${target}")
    endforeach()

    if (SPACEPI_AUXTAR_LINK)
        target_link_libraries("${SPACEPI_AUXTAR_NAME}" PUBLIC ${SPACEPI_AUXTAR_LINK})
    endif()

    if (SPACEPI_AUXTAR_EXAMPLE)
        add_dependencies(examples "${SPACEPI_AUXTAR_NAME}")
    elseif (SPACEPI_AUXTAR_ALL_TARGET)
        add_dependencies("${SPACEPI_AUXTAR_ALL_TARGET}" "${SPACEPI_AUXTAR_NAME}")
    endif()
endfunction()
