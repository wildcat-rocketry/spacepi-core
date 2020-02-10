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
install(FILES blinky-ip.service DESTINATION /etc/systemd/system)
install(CODE "execute_process(COMMAND \"systemctl enable blinky-ip.service\")")
