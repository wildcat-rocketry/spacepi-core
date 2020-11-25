cmake_minimum_required(VERSION 3.11)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

option(BUILD_SHARED_LIBS "Build using shared libraries." ON)

set_property(GLOBAL PROPERTY CXX_STANDARD 14)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Protobuf REQUIRED)
find_package(Java 1.8 COMPONENTS Development)

function (spacepi_message_library)
    set(sources ${ARGV})
    list(REMOVE_AT sources 0 1)

    set(cxxSources)
    set(dependencies)
    foreach (source IN LISTS sources)
        get_filename_component(namespace "${source}" DIRECTORY)
        get_filename_component(basename "${source}" NAME_WE)
        list(APPEND cxxSources "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}/${namespace}/${basename}.pb.cc")
        list(APPEND dependencies "${ARGV1}/${source}")
    endforeach()

    add_library("${ARGV0}" STATIC ${cxxSources})
    target_include_directories("${ARGV0}" PUBLIC "${ARGV1}" "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}" ${SPACEPI_CORE_MESSAGES_INCLUDE})

    set(outputs --cpp_out "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}")
    if (Java_Development_FOUND)
        list(APPEND outputs --java_out "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}/java")

        configure_file("${SPACEPI_CORE_SOURCE_DIR}/cmake/data/build.gradle" "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}/build.gradle")
        configure_file("${SPACEPI_CORE_SOURCE_DIR}/cmake/data/gradle-wrapper.jar" "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}/gradle/wrapper/gradle-wrapper.jar" COPYONLY)
        configure_file("${SPACEPI_CORE_SOURCE_DIR}/cmake/data/gradle-wrapper.properties" "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}/gradle/wrapper/gradle-wrapper.properties" COPYONLY)
        configure_file("${SPACEPI_CORE_SOURCE_DIR}/cmake/data/gradlew" "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}/gradlew" COPYONLY)
        configure_file("${SPACEPI_CORE_SOURCE_DIR}/cmake/data/gradlew.bat" "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}/gradlew.bat" COPYONLY)

        if (WIN32)
            set(gradlew ./gradlew.bat)
        else()
            set(gradlew ./gradlew)
        endif()

        set(javaDependencies "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}/protobuf.end")
        if (NOT "${ARGV0}" STREQUAL "core-messages")
            list(APPEND javaDependencies "core-messages-java")
        endif()

        add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}/${ARGV0}.jar"
            COMMAND ${gradlew}
            ARGS build --warning-mode all
            DEPENDS ${javaDependencies}
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}"
            COMMENT "Running Java compiler on ${ARGV0}"
            VERBATIM
        )

        add_custom_target("${ARGV0}-java" ALL
            DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}/${ARGV0}.jar"
        )

        if (NOT SPACEPI_CORE_MESSAGES_JAR)
            set(SPACEPI_CORE_MESSAGES_JAR "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}/${ARGV0}.jar" CACHE INTERNAL "")
        endif()
    endif()

    add_custom_command(
        OUTPUT ${cxxSources} "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}/protobuf.begin" "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}/protobuf.end"
        COMMAND ${CMAKE_COMMAND}
        ARGS -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}/java"
        COMMAND ${CMAKE_COMMAND}
        ARGS -E touch "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}/protobuf.begin"
        COMMAND protobuf::protoc
        ARGS ${outputs} "-I$<JOIN:$<TARGET_PROPERTY:${ARGV0},INCLUDE_DIRECTORIES>,;-I>" ${sources}
        COMMAND ${CMAKE_COMMAND}
        ARGS -E touch "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}/protobuf.end"
        COMMAND ${CMAKE_COMMAND}
        ARGS "-Droot_dir=${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}" "-Dexts=java,pb.cc,pb.h" "-Dcomp_file=${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}/protobuf.begin" -P "${SPACEPI_CORE_SOURCE_DIR}/cmake/CleanProtobuf.cmake"
        DEPENDS protobuf::protoc ${dependencies}
        COMMENT "Running protocol buffer compiler on ${ARGV0}"
        VERBATIM
        COMMAND_EXPAND_LISTS
    )
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}")

    if (NOT SPACEPI_CORE_MESSAGES_INCLUDE)
        set(SPACEPI_CORE_MESSAGES_INCLUDE
            "${SPACEPI_CORE_SOURCE_DIR}/include"
            "${CMAKE_CURRENT_BINARY_DIR}/${ARGV0}"
            CACHE INTERNAL ""
        )
    endif()
endfunction()

function (spacepi_current_module)
    get_filename_component(modName "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
    file(RELATIVE_PATH relModDir "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    string(REGEX REPLACE "[/\\]" "_" modPrefix "${relModDir}")
    set(${ARGV0} "spacepi-mod_${modPrefix}_${modName}" PARENT_SCOPE)
endfunction()

function (spacepi_module)
    spacepi_current_module(moduleName)

    add_executable("${moduleName}" ${ARGV})
    target_precompile_headers("${moduleName}" REUSE_FROM spacepi_pch)
    target_link_libraries("${moduleName}" PUBLIC spacepi)

    install(TARGETS "${moduleName}" RUNTIME DESTINATION bin)
endfunction()

function (spacepi_module_include_directories)
    spacepi_current_module(moduleName)
    
    target_include_directories(${moduleName} ${ARGV})
endfunction()

function (spacepi_module_precompile_headers)
    spacepi_current_module(moduleName)

    target_precompile_headers(${moduleName} ${ARGV})
endfunction()

function (spacepi_module_link_libraries)
    spacepi_current_module(moduleName)

    target_link_libraries(${moduleName} ${ARGV})
endfunction()

function (spacepi_target_in_dir)
    file(RELATIVE_PATH relTargetDir "${CMAKE_SOURCE_DIR}" "${ARGV1}")
    string(REGEX REPLACE "[/\\]" "_" targetName "${relTargetDir}")
    if ("${targetName}" STREQUAL ".")
        get_filename_component(targetName "${ARGV1}" NAME)
    endif()
    set(${ARGV0} "spacepi-target_${targetName}" PARENT_SCOPE)
endfunction()

function (spacepi_current_target)
    spacepi_target_in_dir(tmp "${CMAKE_CURRENT_SOURCE_DIR}")
    set(${ARGV0} "${tmp}" PARENT_SCOPE)
endfunction()

function (spacepi_target)
    spacepi_current_target(targetName)

    add_executable("${targetName}" ${ARGV})
    target_precompile_headers("${targetName}" REUSE_FROM spacepi_pch)
    target_link_libraries("${targetName}" PUBLIC spacepi)
endfunction()

function (spacepi_target_include_directories)
    spacepi_current_target(targetName)

    target_include_directories("${targetName}" ${ARGV})
endfunction()

function (spacepi_target_link_libraries)
    spacepi_current_target(targetName)

    target_link_libraries("${targetName}" ${ARGV})
endfunction()

function (spacepi_package)
    set(args ${ARGV})
    list(REMOVE_AT args 0 1 2 3)

    spacepi_target_in_dir(targetName "${CMAKE_CURRENT_SOURCE_DIR}/${ARGV1}")

    add_custom_target(${ARGV0} ALL
        COMMAND "$<TARGET_FILE:${targetName}>" --config-file "${CMAKE_CURRENT_SOURCE_DIR}/${ARGV2}" --out "${CMAKE_CURRENT_BINARY_DIR}/${ARGV3}" --data-dir "${CMAKE_CURRENT_SOURCE_DIR}/${ARGV1}" ${args}
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        COMMENT "Updating package ${ARGV0}"
        VERBATIM
    )
endfunction()

function (spacepi_current_extension)
    get_filename_component(extName "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
    file(RELATIVE_PATH relExtDir "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    string(REGEX REPLACE "[/\\]" "_" extPrefix "${relExtDir}")
    set(${ARGV0} "spacepi-ext_${extPrefix}_${extName}" PARENT_SCOPE)
endfunction()

function (spacepi_extension)
    spacepi_current_extension(extensionName)

    add_library("${extensionName}" ${ARGV})
    target_precompile_headers("${extensionName}" REUSE_FROM spacepi)
    target_link_libraries("${extensionName}" PUBLIC spacepi)

    install(TARGETS "${extensionName}" LIBRARY DESTINATION lib)
endfunction()

function (spacepi_extension_include_directories)
    spacepi_current_extension(extensionName)

    target_include_directories(${extensionName} ${ARGV})
endfunction()

function (spacepi_extension_precompile_headers)
    spacepi_current_extension(extensionName)

    target_precompile_headers(${extensionName} ${ARGV})
endfunction()


function (spacepi_extension_link_libraries)
    spacepi_current_extension(extensionName)

    target_link_libraries(${extensionName} ${ARGV})
endfunction()
