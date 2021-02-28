if (WIN32)
    set(gradlew ./gradlew.bat)
else()
    set(gradlew ./gradlew)
endif()

spacepi_dependencies(
    OPTIONAL
    JAVA_JDK
)

if (TARGET JAVA_JDK)
    add_custom_target(dashboard ALL
        COMMAND ${gradlew} --project-cache-dir "${CMAKE_CURRENT_BINARY_DIR}/.gradle" "-PbuildDir=${CMAKE_CURRENT_BINARY_DIR}" build
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS java_message_libraries
    )

    if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        add_dependencies(default dashboard)
    endif()
endif()
