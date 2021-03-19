spacepi_dependencies(
    BOOST_FILESYSTEM
    BOOST_SYSTEM
    BOOST_THREAD
    GIT_LIB
    SYSTEMCTL
)

get_target_property(package_list packages SPACEPI_PKG_LIST)
if (NOT package_list STREQUAL "")
    string(REPLACE ";" "\", \"" SPACEPI_PACKAGES_FILES "${package_list}")
endif()

get_target_property(SYSTEMCTL_EXECUTABLE SYSTEMCTL IMPORTED_LOCATION)

spacepi_aux_program(
    spacepictl
    SOURCES
        src/util/FSOperation.cpp
        src/util/FSOStream.cpp
        src/util/FSTransaction.cpp
        src/util/Person.cpp
        src/util/System.cpp
        src/util/User.cpp
        src/util/UserManager.cpp
        src/ConfigSet.cpp
        src/Exec.cpp
        src/Help.cpp
        src/Init.cpp
        src/List.cpp
        src/RemoteID.cpp
        src/Systemctl.cpp
        src/Verb.cpp
    CONFIG_FILE include/spacepi/spacepictl/Config.hpp
    INCLUDE include
    PRECOMPILE_FROM spacepi_pch
    LINK
        spacepi
        BOOST_FILESYSTEM
        BOOST_SYSTEM
        BOOST_THREAD
        GIT_LIB
)

configure_file("etc/spacepi-sshgen.service" "${CMAKE_CURRENT_BINARY_DIR}/etc/spacepi-sshgen.service" @ONLY)

install(
    FILES
        "bin/spacepi-sshgen"
    DESTINATION bin
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/etc/spacepi-sshgen.service"
    DESTINATION /usr/lib/systemd/system
)

install(
    CODE
        "execute_process(COMMAND systemctl enable
            spacepi-sshgen.service
        )"
)
