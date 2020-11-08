add_executable(spacepi-init
    src/main.cpp
)

find_package(Boost
    REQUIRED COMPONENTS
    # asio
    # beast
    system
    thread
)

target_link_libraries(setup-deploy-key
    Boost::system
    Boost::thread
)

target_include_directories(spacepi-init PUBLIC include)
