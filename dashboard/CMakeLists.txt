if (WIN32)
    set(gradlew ./gradlew.bat)
else()
    set(gradlew ./gradlew)
endif()

find_package(Java 1.8 COMPONENTS Development)

if (Java_Development_FOUND)
    add_custom_target(dashboard ALL
        COMMAND ${gradlew} --project-cache-dir "${CMAKE_CURRENT_BINARY_DIR}/.gradle" "-PbuildDir=${CMAKE_CURRENT_BINARY_DIR}" build
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()
