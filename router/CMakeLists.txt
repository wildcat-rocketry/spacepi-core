spacepi_dependencies(
    BOOST_ASIO
    BOOST_FILESYSTEM
    BOOST_SYSTEM
)

spacepi_module(
    SOURCES
        src/main.cpp
        src/PubSubEndpoint.cpp
        src/PubSubRouter.cpp
        src/Router.cpp
        src/RouterEndpoint.cpp
        src/StreamClient.cpp
        src/StreamServer.cpp
        src/UnixStreamServerWrapper.cpp
    INCLUDE include
    PRECOMPILE_FROM spacepi_pch
    LINK
        BOOST_ASIO
        BOOST_FILESYSTEM
        BOOST_SYSTEM
)
