cmake_minimum_required(VERSION 3.0)
project(spacepi VERSION 1.0)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_library(spacepi
    src/io/gpio.c
    src/io/io.c
    src/io/nc.c
    src/io/PCA9571.c
    src/pubsub/allocation.c
    src/pubsub/cleanup.c
    src/pubsub/connect.c
    src/pubsub/publish.c
    src/pubsub/subscribe.c
    src/error.c
    src/random.c
    src/thread_pool.c
    src/timing.c
)
target_include_directories(spacepi PUBLIC
    "${PROJECT_SOURCE_DIR}/include/public"
)
target_include_directories(spacepi PRIVATE
    "${PROJECT_SOURCE_DIR}/include/private"
)
target_link_libraries(spacepi paho-mqtt3a paho-mqtt3c)
install(TARGETS spacepi DESTINATION .)

add_subdirectory(test)
