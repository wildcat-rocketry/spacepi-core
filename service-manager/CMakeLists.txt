cmake_minimum_required(VERSION 3.0)
project(service-manager VERSION 1.0)

add_executable(service-manager
    main.c
)

target_link_libraries(service-manager PUBLIC spacepi)
install(TARGETS service-manager DESTINATION .)
install(FILES service-manager.service DESTINATION /etc/systemd/system)
install(CODE "execute_process(COMMAND systemctl enable service-manager.service)")
