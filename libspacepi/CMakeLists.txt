cmake_minimum_required(VERSION 3.0)
project(spacepi VERSION 1.0)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_library(spacepi
    src/pubsub.c
    src/random.c
)
target_include_directories(spacepi PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries(spacepi paho-mqtt3a paho-mqtt3c)

add_subdirectory(test)
