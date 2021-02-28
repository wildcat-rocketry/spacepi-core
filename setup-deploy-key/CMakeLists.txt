spacepi_dependencies(
    BOOST_ASIO
    BOOST_BEAST
    BOOST_FILESYSTEM
    BOOST_SYSTEM
    BOOST_THREAD
    OPENSSL
    OPENSSL_CRYPTO
)

spacepi_aux_program(
    setup-deploy-key
    SOURCES
        src/main.cpp
        src/Server.cpp
        src/ServerConn.cpp
        src/GithubHandler.cpp
        src/Client.cpp
        src/DeployKeySetup.cpp
    INCLUDE include
    PRECOMPILE_FROM spacepi_pch
    LINK
        spacepi
        BOOST_ASIO
        BOOST_BEAST
        BOOST_FILESYSTEM
        BOOST_SYSTEM
        BOOST_THREAD
        OPENSSL
        OPENSSL_CRYPTO
)
