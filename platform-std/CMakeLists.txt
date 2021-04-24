include(SpacePiPlatform)
spacepi_platform(DEFAULT)

include(SpacePiTargets)

# spacepi_module(
#     [CORE | EXAMPLE]
#     MODULE | LIBRARY | MESSAGES
#     [NAME <name>]
#     SOURCES <source 1> [<source 2> ...]
#     [CONFIG_FILE <file>]
#     [INCLUDE <dir 1> [<dir 2> ...]]
#     [PRECOMPILE <file 1> [<file 2> ...]]
#     [LINK <lib 1> [<lib 2> ...]]
# )
function (spacepi_module)
    cmake_parse_arguments(SPACEPI_MOD "MODULE;LIBRARY;MESSAGES;CORE;EXAMPLE" "NAME;CONFIG_FILE" "SOURCES;INCLUDE;PRECOMPILE;LINK" ${ARGN})

    set(type)
    set(allTarget)
    if (SPACEPI_MOD_MODULE)
        if (SPACEPI_MOD_LIBRARY OR SPACEPI_MOD_MESSAGES)
            message(FATAL_ERROR "Invalid usage of spacepi_module()")
        endif()
        set(type "mod")
        set(allTarget "modules")
    elseif (SPACEPI_MOD_LIBRARY)
        if (SPACEPI_MOD_MESSAGES)
            message(FATAL_ERROR "Invalid usage of spacepi_module()")
        endif()
        set(type "lib")
        set(allTarget "libraries")
    elseif (SPACEPI_MOD_MESSAGES)
        set(type "proto")
        set(allTarget "messages")
    else()
        message(FATAL_ERROR "Invalid usage of spacepi_module()")
    endif()

    if (SPACEPI_MOD_CORE)
        if (SPACEPI_MOD_EXAMPLE)
            message(FATAL_ERROR "Invalid usage of spacepi_module()")
        endif()
        set(type "${type}-core")
        set(allTarget "core")
    elseif (SPACEPI_MOD_EXAMPLE)
        set(type "${type}-example")
        set(allTarget "examples")
    endif()

    if (NOT SPACEPI_MOD_NAME)
        file(RELATIVE_PATH relDir "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
        string(REGEX REPLACE "[^-.0-9A-Za-z]" "_" compName "${relDir}")
        if (compName STREQUAL "")
            set(SPACEPI_MOD_NAME "spacepi-${type}")
        else()
            set(SPACEPI_MOD_NAME "spacepi-${type}-${compName}")
        endif()
    endif()

    if (SPACEPI_MOD_MODULE)
        add_executable("${SPACEPI_MOD_NAME}" EXCLUDE_FROM_ALL "${SPACEPI_MOD_SOURCES}")
    elseif (SPACEPI_MOD_LIBRARY)
        add_library("${SPACEPI_MOD_NAME}" SHARED EXCLUDE_FROM_ALL "${SPACEPI_MOD_SOURCES}")
    elseif (SPACEPI_MOD_MESSAGES)
        message(FATAL_ERROR "Not implemented")
    endif()

    add_dependencies("${allTarget}" "${SPACEPI_MOD_NAME}")

    if (SPACEPI_MOD_CONFIG_FILE)
        configure_file("${SPACEPI_MOD_CONFIG_FILE}.in" "${CMAKE_CURRENT_BINARY_DIR}/${SPACEPI_MOD_CONFIG_FILE}")
        set(found FALSE)
        foreach (dir IN LISTS SPACEPI_MOD_INCLUDE)
            string(LENGTH "${dir}" dirLen)
            string(SUBSTRING "${SPACEPI_MOD_CONFIG_FILE}" "0" "${dirLen}" includeDir)
            if (includeDir STREQUAL dir)
                target_include_directories("${SPACEPI_MOD_NAME}" PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/${dir}")
                set(found TRUE)
            endif()
        endforeach()
        if (NOT found)
            target_include_directories("${SPACEPI_MOD_NAME}" PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
        endif()
    endif()

    if (SPACEPI_MOD_INCLUDE)
        target_include_directories("${SPACEPI_MOD_NAME}" PUBLIC ${SPACEPI_MOD_INCLUDE})
    endif()

    foreach (file IN LISTS SPACEPI_MOD_PRECOMPILE)
        target_precompile_headers("${SPACEPI_MOD_NAME}" PUBLIC "${file}")
    endforeach()

    if (SPACEPI_MOD_LINK)
        target_link_libraries("${SPACEPI_MOD_NAME}" PUBLIC ${SPACEPI_MOD_LINK})
    endif()
endfunction()

spacepi_module(
    CORE LIBRARY NAME spacepi-platform
    SOURCES
        src/empty.cpp
    INCLUDE include ../include
)
