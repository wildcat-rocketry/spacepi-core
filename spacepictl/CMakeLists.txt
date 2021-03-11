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
        src/util/FSOperation.cpp
        src/util/FSOStream.cpp
        src/util/FSTransaction.cpp
        src/util/Person.cpp
        src/util/System.cpp
        src/util/User.cpp
        src/util/UserManager.cpp
        src/SpacePiCTL.cpp
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
