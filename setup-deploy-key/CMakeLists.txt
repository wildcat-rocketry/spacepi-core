add_executable(setup-deploy-key
    src/main.cpp
    src/Server.cpp
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
    spacepi
)
target_include_directories(setup-deploy-key PUBLIC include)
