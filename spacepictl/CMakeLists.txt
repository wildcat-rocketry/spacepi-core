spacepi_dependencies(
    BOOST_FILESYSTEM
    BOOST_SYSTEM
    BOOST_THREAD
    GIT_LIB
)

get_target_property(package_list packages SPACEPI_PKG_LIST)
if (NOT package_list STREQUAL "")
    string(REPLACE ";" "\", \"" SPACEPI_PACKAGES_FILES "${package_list}")
endif()

spacepi_aux_program(
    spacepictl
    SOURCES
        src/FSOperation.cpp
        src/FSOStream.cpp
        src/FSTransaction.cpp
        src/SpacePiCTL.cpp
        src/Person.cpp
        src/User.cpp
        src/UserManager.cpp
        src/System.cpp
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
