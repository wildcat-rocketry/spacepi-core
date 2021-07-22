cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0091 NEW)
project(spacepi-core)
include(cmake/SpacePiSetup.cmake)

include(SpacePiOnce)
spacepi_once(SPACEPI_CORE)

add_subdirectory(dashboard)
add_subdirectory(protoc)

find_package(Doxygen)
find_package(Python3 COMPONENTS Interpreter)
find_package(Pip3)
if (TARGET Doxygen::doxygen AND TARGET Doxygen::dot AND TARGET Python3::Interpreter AND Pip3_FOUND)
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/sphinx.stamp"
        COMMAND Python3::Interpreter
        ARGS -m pip install -q --user -r requirements.txt
        COMMAND ${CMAKE_COMMAND}
        ARGS -E env "BUILD_DIR=${CMAKE_CURRENT_BINARY_DIR}" $<TARGET_FILE:Python3::Interpreter> -m sphinx.cmd.build -b html -c . "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/sphinx"
        COMMAND ${CMAKE_COMMAND}
        ARGS -E touch "${CMAKE_CURRENT_BINARY_DIR}/sphinx.stamp"
        DEPENDS
            docs/conf.py
            docs/architecture.rst
            docs/core-contrib.rst
            docs/getting-started.rst
            docs/networking.rst
            docs/requirements.txt
            docs/submodules.rst
            docs/troubleshooting.rst
            include/spacepi/platform/Init.hpp
            include/spacepi/Platform.hpp
            include/SpacePi.hpp
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/docs"
        COMMENT "Generating SpacePi Sphinx documentation"
        VERBATIM
    )
    add_custom_target("spacepi-docs" ALL
        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/sphinx.stamp"
    )
endif()
