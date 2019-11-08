cmake_minimum_required(VERSION 3.0)
project(libspacepi-test VERSION 1.0)

add_executable(libspacepi-test
    main.c
)

target_link_libraries(libspacepi-test PUBLIC spacepi)
