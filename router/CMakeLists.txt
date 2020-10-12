find_package(Boost
    REQUIRED COMPONENTS
    # asio
    system
)

spacepi_module(
    src/main.cpp
    src/PubSubEndpoint.cpp
    src/PubSubRouter.cpp
    src/StreamClient.cpp
    src/StreamServer.cpp
)
spacepi_module_include_directories(PUBLIC include)
spacepi_module_link_libraries(PUBLIC
    Boost::system
    core-messages
)
