find_package(Doxygen)
find_package(Python3 COMPONENTS Interpreter)
find_package(Pip3)
if (TARGET Doxygen::doxygen AND TARGET Doxygen::dot AND TARGET Python3::Interpreter AND Pip3_FOUND)
    get_property(loc TARGET Doxygen::doxygen PROPERTY IMPORTED_LOCATION)
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/sphinx.stamp"
        COMMAND Python3::Interpreter
        ARGS -m pip install -q --user -r requirements.txt
        COMMAND ${CMAKE_COMMAND}
        ARGS -E env "BUILD_DIR=${CMAKE_CURRENT_BINARY_DIR}" "DOXYGEN_EXE=${loc}" $<TARGET_FILE:Python3::Interpreter> -m sphinx.cmd.build -T -b html -c . "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/sphinx"
        COMMAND ${CMAKE_COMMAND}
        ARGS -E touch "${CMAKE_CURRENT_BINARY_DIR}/sphinx.stamp"
        DEPENDS
            static/architecture.rst
            static/core-contrib.rst
            static/getting-started.rst
            static/networking.rst
            static/submodules.rst
            static/troubleshooting.rst
            conf.py
            Doxyfile.in
            requirements.txt
            spacepi-pch
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Generating SpacePi Sphinx documentation"
        VERBATIM
    )
    add_custom_target("spacepi-docs" ALL
        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/sphinx.stamp"
    )
endif()
