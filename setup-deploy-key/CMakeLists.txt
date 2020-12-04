add_executable(setup-deploy-key
    src/main.cpp
    src/Server.cpp
    src/ServerConn.cpp
    src/GithubHandler.cpp
    src/Client.cpp
    src/DeployKeySetup.cpp
)

find_package(Boost
    REQUIRED COMPONENTS
    # asio
    # beast
    system
    thread
    filesystem
)

find_package(OpenSSL REQUIRED)

target_link_libraries(setup-deploy-key
    Boost::system
    Boost::thread
    Boost::filesystem
    OpenSSL::SSL
    spacepi
)
target_include_directories(setup-deploy-key PUBLIC include)

target_precompile_headers(setup-deploy-key REUSE_FROM spacepi_pch)
