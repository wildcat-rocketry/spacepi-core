cmake_minimum_required(VERSION 3.0)
project(blinky-ip VERSION 1.0)

add_executable(blinky-ip
    ips.c
    main.c
)

target_link_libraries(blinky-ip PUBLIC spacepi)
install(TARGETS blinky-ip DESTINATION .)
