cmake_minimum_required(VERSION 3.0)
project(blinky-ip VERSION 1.0)

add_executable(blinky-ip
    encoder.c
    ips.c
    led.c
    main.c
    timing.c
)

target_link_libraries(blinky-ip PUBLIC spacepi)
install(TARGETS blinky-ip DESTINATION .)
