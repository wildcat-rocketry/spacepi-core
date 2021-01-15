find_package(Boost
    REQUIRED COMPONENTS
    # asio
    filesystem
    system
)

spacepi_module(
    src/main.cpp
    src/PubSubEndpoint.cpp
    src/PubSubRouter.cpp
    src/Router.cpp
    src/RouterEndpoint.cpp
    src/StreamClient.cpp
    src/StreamServer.cpp
    src/UnixStreamServerWrapper.cpp
)
spacepi_module_include_directories(PUBLIC include)
spacepi_module_link_libraries(PUBLIC
    Boost::filesystem
    Boost::system
    core-messages
    ${CMAKE_DL_LIBS}
)
