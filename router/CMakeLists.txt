find_package(Boost
    REQUIRED COMPONENTS
    # asio
    system
)

spacepi_module(
    src/Client.cpp
    src/main.cpp
    src/RouterServer.cpp
)
spacepi_module_include_directories(PUBLIC include)
spacepi_module_link_libraries(PUBLIC Boost::system)
