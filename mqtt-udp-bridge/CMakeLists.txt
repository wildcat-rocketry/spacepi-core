cmake_minimum_required(VERSION 3.0)
project(mqtt-udp-bridge VERSION 1.0)

add_executable(mqtt-udp-bridge
    main.c
)

target_link_libraries(mqtt-udp-bridge PUBLIC spacepi)
