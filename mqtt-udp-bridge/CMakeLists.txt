cmake_minimum_required(VERSION 3.0)
project(mqtt-udp-bridge VERSION 1.0)

#option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_library(spacepi
    src/pubsub.c
)
target_include_directories(mqtt-udp-bridge PUBLIC
    "${PROJECT_SOURCE_DIR}"
)
