spacepi_dependencies(
    PROTOC
    LIBPROTOBUF
)
spacepi_dependencies(
    OPTIONAL
    JAVA_JDK
)

define_property(TARGET PROPERTY SPACEPI_MLIB_JAR BRIEF_DOCS "." FULL_DOCS ".")
define_property(TARGET PROPERTY SPACEPI_MLIB_TARGET BRIEF_DOCS "." FULL_DOCS ".")

add_custom_target(cxx_message_libraries)
add_custom_target(java_message_libraries)
add_custom_target(message_libraries DEPENDS
    cxx_message_libraries
    java_message_libraries
)
add_dependencies(full message_libraries)

if (WIN32)
    set(SPACEPI_MLIB_GRADLEW "./gradlew.bat")
else()
    set(SPACEPI_MLIB_GRADLEW "./gradlew")
endif()
get_target_property(SPACEPI_MLIB_PROTOC PROTOC IMPORTED_LOCATION)
if (TARGET JAVA_JDK)
    set(SPACEPI_MLIB_HAS_JAVA TRUE)
else()
    set(SPACEPI_MLIB_HAS_JAVA FALSE)
endif()

# spacepi_message_library(
#     <name> <source dir>
#     SOURCES <source1> [<source2> ...]
#     [DEPENDS <dep1> [<dep2> ...]]
#     [EXAMPLE]
#     [NO_CORE_DEP]
# )
function (spacepi_message_library SPACEPI_MLIB_NAME SPACEPI_MLIB_DIR)
    cmake_parse_arguments(SPACEPI_MLIB "EXAMPLE;NO_CORE_DEP" "" "SOURCES;DEPENDS" ${ARGN})
    if (NOT SPACEPI_MLIB_NO_CORE_DEP)
        list(APPEND SPACEPI_MLIB_DEPENDS "core-messages")
    endif()

    set(cxxSources)
    set(protoSources)
    foreach (source IN LISTS SPACEPI_MLIB_SOURCES)
        get_filename_component(namespace "${source}" DIRECTORY)
        get_filename_component(basename "${source}" NAME_WE)
        list(APPEND cxxSources "${CMAKE_CURRENT_BINARY_DIR}/${SPACEPI_MLIB_NAME}/${namespace}/${basename}.pb.cc")
        list(APPEND protoSources "${CMAKE_CURRENT_SOURCE_DIR}/${SPACEPI_MLIB_DIR}/${source}")
    endforeach()

    add_library("${SPACEPI_MLIB_NAME}" STATIC EXCLUDE_FROM_ALL ${cxxSources})
    target_include_directories("${SPACEPI_MLIB_NAME}" PUBLIC "${SPACEPI_MLIB_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/${SPACEPI_MLIB_NAME}")
    target_link_libraries("${SPACEPI_MLIB_NAME}" ${SPACEPI_MLIB_DEPENDS})

    if (SPACEPI_MLIB_EXAMPLE)
        add_dependencies(examples "${SPACEPI_MLIB_NAME}")
    else()
        add_dependencies(cxx_message_libraries "${SPACEPI_MLIB_NAME}")
    endif()

    if (TARGET JAVA_JDK)
        set(depJars)
        set(depTargets)
        foreach (dep IN LISTS SPACEPI_MLIB_DEPENDS)
            get_target_property(depJar "${dep}" SPACEPI_MLIB_JAR)
            list(APPEND depJars "${depJar}")

            get_target_property(depTarget "${dep}" SPACEPI_MLIB_TARGET)
            list(APPEND depTargets "${depTarget}")
        endforeach()

        configure_file("${SPACEPI_CORE_SOURCE_DIR}/cmake/data/build.gradle" "${CMAKE_CURRENT_BINARY_DIR}/${SPACEPI_MLIB_NAME}/build.gradle")
        configure_file("${SPACEPI_CORE_SOURCE_DIR}/cmake/data/gradle-wrapper.jar" "${CMAKE_CURRENT_BINARY_DIR}/${SPACEPI_MLIB_NAME}/gradle/wrapper/gradle-wrapper.jar" COPYONLY)
        configure_file("${SPACEPI_CORE_SOURCE_DIR}/cmake/data/gradle-wrapper.properties" "${CMAKE_CURRENT_BINARY_DIR}/${SPACEPI_MLIB_NAME}/gradle/wrapper/gradle-wrapper.properties" COPYONLY)
        configure_file("${SPACEPI_CORE_SOURCE_DIR}/cmake/data/gradlew" "${CMAKE_CURRENT_BINARY_DIR}/${SPACEPI_MLIB_NAME}/gradlew" COPYONLY)
        configure_file("${SPACEPI_CORE_SOURCE_DIR}/cmake/data/gradlew.bat" "${CMAKE_CURRENT_BINARY_DIR}/${SPACEPI_MLIB_NAME}/gradlew.bat" COPYONLY)

        add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${SPACEPI_MLIB_NAME}/${SPACEPI_MLIB_NAME}.jar"
            COMMAND "${SPACEPI_MLIB_GRADLEW}"
            ARGS "build" "--warning-mode" "all"
            DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${SPACEPI_MLIB_NAME}/protobuf.stamp" ${depTargets}
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${SPACEPI_MLIB_NAME}"
            COMMENT "Running Java compiler on ${SPACEPI_MLIB_NAME}"
            VERBATIM
        )

        add_custom_target(
            "java_message_library_${SPACEPI_MLIB_NAME}"
            DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${SPACEPI_MLIB_NAME}/${SPACEPI_MLIB_NAME}.jar"
        )

        if (SPACEPI_MLIB_EXAMPLE)
            add_dependencies(examples "java_message_library_${SPACEPI_MLIB_NAME}")
        else()
            add_dependencies(java_message_libraries "java_message_library_${SPACEPI_MLIB_NAME}")
        endif()

        set_target_properties("${SPACEPI_MLIB_NAME}" PROPERTIES SPACEPI_MLIB_JAR "${CMAKE_CURRENT_BINARY_DIR}/${SPACEPI_MLIB_NAME}/${SPACEPI_MLIB_NAME}.jar")
        set_target_properties("${SPACEPI_MLIB_NAME}" PROPERTIES SPACEPI_MLIB_TARGET "java_message_library_${SPACEPI_MLIB_NAME}")
    endif()

    add_custom_command(
        OUTPUT ${cxxSources} "${CMAKE_CURRENT_BINARY_DIR}/${SPACEPI_MLIB_NAME}/protobuf.stamp"
        COMMAND "${CMAKE_COMMAND}"
        ARGS
            "-DSPACEPI_MLIB_JAVA=${SPACEPI_MLIB_HAS_JAVA}"
            "-DSPACEPI_MLIB_PROTOC=${SPACEPI_MLIB_PROTOC}"
            "-DSPACEPI_MLIB_OUTDIR=${CMAKE_CURRENT_BINARY_DIR}/${SPACEPI_MLIB_NAME}"
            "-DSPACEPI_MLIB_INCLUDE=-I$<JOIN:$<TARGET_PROPERTY:${SPACEPI_MLIB_NAME},INCLUDE_DIRECTORIES>,;-I>"
            "-DSPACEPI_MLIB_SOURCES=${protoSources}"
            "-P" "${SPACEPI_CORE_SOURCE_DIR}/cmake/components/Protobuf.cmake"
        DEPENDS PROTOC ${protoSources} ${SPACEPI_MLIB_DEPENDS} "${SPACEPI_CORE_SOURCE_DIR}/cmake/components/Protobuf.cmake"
        COMMENT "Running protocol buffer compiler on ${SPACEPI_MLIB_NAME}"
        VERBATIM
    )
endfunction()
