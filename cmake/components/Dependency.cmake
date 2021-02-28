# spacepi_dependency_message(
#     <type>
#     <component>
# )
function (spacepi_dependency_message SPACEPI_DEP_MSG_TYPE SPACEPI_DEP_MSG_COMPONENT)
    list(GET SPACEPI_DEP_MSG_COMPONENT "0" firstName)

    set(msg "Looking for ${firstName}")
    set(stat FALSE)
    if (SPACEPI_DEP_MSG_TYPE STREQUAL CHECK_PASS)
        set(stat "found")
    elseif (SPACEPI_DEP_MSG_TYPE STREQUAL CHECK_FAIL)
        set(stat "not found")
    endif()

    list(LENGTH SPACEPI_DEP_MSG_STACK stackSize)
    math(EXPR stackEnd "${stackSize} - 1")

    if (stat)
        list(GET SPACEPI_DEP_MSG_QUIET "${stackEnd}" quiet)
        list(REMOVE_AT SPACEPI_DEP_MSG_QUIET "${stackEnd}")
        list(REMOVE_AT SPACEPI_DEP_MSG_STACK "${stackEnd}")
    else()
        if (stackEnd GREATER_EQUAL 0)
            list(GET SPACEPI_DEP_MSG_STACK "${stackEnd}" lastName)
            if (lastName STREQUAL firstName)
                set(quiet TRUE)
            else()
                set(quiet FALSE)
            endif()
        else()
            set(quiet FALSE)
        endif()

        list(APPEND SPACEPI_DEP_MSG_QUIET "${quiet}")
        list(APPEND SPACEPI_DEP_MSG_STACK "${firstName}")
    endif()

    set(SPACEPI_DEP_MSG_QUIET "${SPACEPI_DEP_MSG_QUIET}" PARENT_SCOPE)
    set(SPACEPI_DEP_MSG_STACK "${SPACEPI_DEP_MSG_STACK}" PARENT_SCOPE)

    if (NOT quiet)
        if (stat)
            list(LENGTH CMAKE_MESSAGE_INDENT indentSize)
            math(EXPR indentEnd "${indentSize} - 1")
            list(REMOVE_AT CMAKE_MESSAGE_INDENT "${indentEnd}")
        endif()

        if (CMAKE_VERSION VERSION_LESS "3.17")
            if (stat)
                message(STATUS ${CMAKE_MESSAGE_INDENT} "${msg} - ${stat}")
            else()
                message(STATUS ${CMAKE_MESSAGE_INDENT} "${msg}")
            endif()
        else()
            if (stat)
                message("${SPACEPI_DEP_MSG_TYPE}" "${stat}")
            else()
                message("${SPACEPI_DEP_MSG_TYPE}" "${msg}")
            endif()
        endif()
        
        if (NOT stat)
        list(APPEND CMAKE_MESSAGE_INDENT "  ")
        endif()

        set(CMAKE_MESSAGE_INDENT "${CMAKE_MESSAGE_INDENT}" PARENT_SCOPE)
    endif()
endfunction()

# spacepi_dependency_fail(
#     <name>
#     [OPTIONAL]
#     [PACKAGES <package1> [<package2> ...]]
# )
function (spacepi_dependency_fail SPACEPI_DEP_FAIL_NAME)
    cmake_parse_arguments(SPACEPI_DEP_FAIL "OPTIONAL" "" "PACKAGES" ${ARGN})

    if (SPACEPI_DEP_FAIL_OPTIONAL)
        return()
    endif()

    set(str "Could not find required component ${SPACEPI_DEP_FAIL_NAME}.")
    if (SPACEPI_DEP_FAIL_PACKAGES AND SPACEPI_DEP_APT)
        string(APPEND str "\nTry: ${SPACEPI_DEP_APT} install")
        foreach (pkg IN LISTS SPACEPI_DEP_FAIL_PACKAGES)
            string(APPEND str " ${pkg}")
        endforeach()
    endif()
    message(SEND_ERROR "${str}")
endfunction()

# spacepi_dependency_import(
#     <name>
# )
function (spacepi_dependency_import SPACEPI_DEPIMP_NAME)
    if (NOT TARGET "${SPACEPI_DEPIMP_NAME}")
        if (SPACEPI_DEP_${SPACEPI_DEPIMP_NAME}_TYPE)
            add_library("${SPACEPI_DEPIMP_NAME}" "${SPACEPI_DEP_${SPACEPI_DEPIMP_NAME}_TYPE}" IMPORTED GLOBAL)
        else()
            add_executable("${SPACEPI_DEPIMP_NAME}" IMPORTED GLOBAL)
        endif()

        if (SPACEPI_DEP_${SPACEPI_DEPIMP_NAME}_LOCATION)
            set_target_properties("${SPACEPI_DEPIMP_NAME}" PROPERTIES IMPORTED_LOCATION "${SPACEPI_DEP_${SPACEPI_DEPIMP_NAME}_LOCATION}")
        endif()

        if (SPACEPI_DEP_${SPACEPI_DEPIMP_NAME}_INCLUDES)
            target_include_directories("${SPACEPI_DEPIMP_NAME}" INTERFACE ${SPACEPI_DEP_${SPACEPI_DEPIMP_NAME}_INCLUDES})
        endif()
    endif()
endfunction()

# spacepi_dependency(
#     <name>
#     TARGET <target>
#     [PROGRAM_NAMES <name1> [<name2> ...]]
#     [LIBRARY_NAMES <name1> [<name2> ...]]
#     [HEADERS <file1> [<file2> ...]]
#     [PATHS <path1> [<path2> ...]]
#     [OPTIONAL]
#     [PACKAGES <package1> [<package2> ...]]
# )
function (spacepi_dependency SPACEPI_DEP_NAME)
    cmake_parse_arguments(SPACEPI_DEP "OPTIONAL" "TARGET" "PROGRAM_NAMES;LIBRARY_NAMES;HEADERS;PATHS;PACKAGES" ${ARGN})

    if (TARGET "${SPACEPI_DEP_TARGET}")
        return()
    endif()

    if (SPACEPI_DEP_${SPACEPI_DEP_TARGET}_FOUND)
        spacepi_dependency_import("${SPACEPI_DEP_TARGET}")
        return()
    endif()

    spacepi_dependency_message(CHECK_START "${SPACEPI_DEP_NAME}")

    if (SPACEPI_DEP_PROGRAM_NAMES)
        spacepi_dependency_message(CHECK_START "${SPACEPI_DEP_PROGRAM_NAMES}")

        find_program(
            SPACEPI_DEP_FIND_${SPACEPI_DEP_TARGET}
            NAMES ${SPACEPI_DEP_PROGRAM_NAMES}
            PATHS ${SPACEPI_DEP_PATHS}
        )

        if (SPACEPI_DEP_FIND_${SPACEPI_DEP_TARGET})
            spacepi_dependency_message(CHECK_PASS "${SPACEPI_DEP_PROGRAM_NAMES}")

            set(SPACEPI_DEP_${SPACEPI_DEP_TARGET}_FOUND TRUE CACHE INTERNAL "")
            set(SPACEPI_DEP_${SPACEPI_DEP_TARGET}_LOCATION "${SPACEPI_DEP_FIND_${SPACEPI_DEP_TARGET}}" CACHE INTERNAL "")
        else()
            spacepi_dependency_message(CHECK_FAIL "${SPACEPI_DEP_PROGRAM_NAMES}")
        endif()
    else()
        set(valid TRUE)
        set(lib FALSE)
        if (SPACEPI_DEP_LIBRARY_NAMES)
            spacepi_dependency_message(CHECK_START "${SPACEPI_DEP_LIBRARY_NAMES}")

            find_library(
                SPACEPI_DEP_FIND_${SPACEPI_DEP_TARGET}
                NAMES ${SPACEPI_DEP_LIBRARY_NAMES}
                PATHS ${SPACEPI_DEP_PATHS}
            )

            if (SPACEPI_DEP_FIND_${SPACEPI_DEP_TARGET})
                spacepi_dependency_message(CHECK_PASS "${SPACEPI_DEP_LIBRARY_NAMES}")
            else()
                spacepi_dependency_message(CHECK_FAIL "${SPACEPI_DEP_LIBRARY_NAMES}")

                set(valid FALSE)
            endif()
        endif()
        set(includeDirs)
        if (valid)
            foreach (file IN LISTS SPACEPI_DEP_HEADERS)
                spacepi_dependency_message(CHECK_START "${file}")

                find_file(
                    SPACEPI_DEP_FIND_${SPACEPI_DEP_TARGET}_HEADER_${file}
                    NAMES ${file}
                    PATHS ${SPACEPI_DEP_PATHS}
                )

                if (SPACEPI_DEP_FIND_${SPACEPI_DEP_TARGET}_HEADER_${file})
                    spacepi_dependency_message(CHECK_PASS "${file}")

                    string(REGEX REPLACE "${file}$" "" includeDir "${SPACEPI_DEP_FIND_${SPACEPI_DEP_TARGET}_HEADER_${file}}")
                    list(FIND includeDirs "${includeDir}" includeDirRes)
                    if (includeDirRes LESS 0)
                        list(APPEND includeDirs "${includeDir}")
                    endif()
                else()
                    spacepi_dependency_message(CHECK_FAIL "${file}")
    
                    set(valid FALSE)
                endif()
            endforeach()
        endif()
        if (valid)
            if (SPACEPI_DEP_FIND_${SPACEPI_DEP_TARGET})
                get_filename_component(ext "${SPACEPI_DEP_FIND_${SPACEPI_DEP_TARGET}}" EXT)
                if (ext STREQUAL ".so")
                    set(SPACEPI_DEP_${SPACEPI_DEP_TARGET}_TYPE SHARED CACHE INTERNAL "")
                elseif (ext STREQUAL ".a")
                    set(SPACEPI_DEP_${SPACEPI_DEP_TARGET}_TYPE STATIC CACHE INTERNAL "")
                elseif (ext STREQUAL ".lib")
                    set(SPACEPI_DEP_${SPACEPI_DEP_TARGET}_TYPE STATIC CACHE INTERNAL "")
                else()
                    set(SPACEPI_DEP_${SPACEPI_DEP_TARGET}_TYPE UNKNOWN CACHE INTERNAL "")
                endif()
            else()
                set(SPACEPI_DEP_${SPACEPI_DEP_TARGET}_TYPE INTERFACE CACHE INTERNAL "")
            endif()

            if (includeDirs)
                set(SPACEPI_DEP_${SPACEPI_DEP_TARGET}_INCLUDES "${includeDirs}" CACHE INTERNAL "")
            endif()

            set(SPACEPI_DEP_${SPACEPI_DEP_TARGET}_LOCATION "${SPACEPI_DEP_FIND_${SPACEPI_DEP_TARGET}}" CACHE INTERNAL "")
            set(SPACEPI_DEP_${SPACEPI_DEP_TARGET}_FOUND TRUE CACHE INTERNAL "")
        endif()
    endif()

    if (SPACEPI_DEP_${SPACEPI_DEP_TARGET}_FOUND)
        spacepi_dependency_message(CHECK_PASS "${SPACEPI_DEP_NAME}")

        spacepi_dependency_import("${SPACEPI_DEP_TARGET}")
    else()
        spacepi_dependency_message(CHECK_FAIL "${SPACEPI_DEP_NAME}")

        set(args "${SPACEPI_DEP_NAME}")

        if (SPACEPI_DEP_OPTIONAL)
            list(APPEND args OPTIONAL)
        endif()

        if (SPACEPI_DEP_PACKAGES)
            list(APPEND args PACKAGES ${SPACEPI_DEP_PACKAGES})
        endif()

        spacepi_dependency_fail(${args})
    endif()
endfunction()

spacepi_dependency(
    apt
    TARGET APT
    PROGRAM_NAMES apt apt-get
    OPTIONAL
)
if (TARGET APT)
    get_target_property(aptFile APT IMPORTED_LOCATION)
    get_filename_component(SPACEPI_DEP_APT "${aptFile}" NAME_WE)
endif()
