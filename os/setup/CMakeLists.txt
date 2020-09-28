cmake_minimum_required(VERSION 3.0)
project(first-setup VERSION 1.0)

add_executable(first-setup
    setup.cpp
)

target_link_libraries(first-setup PUBLIC {{SOME BOOST}})
install(TARGETS first-setup DESTINATION .)
