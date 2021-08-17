find_package(Python3 COMPONENTS Interpreter)

if (TARGET Python3::Interpreter AND NOT Pip3_FOUND)
    get_property(loc TARGET Python3::Interpreter PROPERTY IMPORTED_LOCATION)
    execute_process(
        COMMAND "${loc}" -m pip help
        RESULT_VARIABLE found
        OUTPUT_QUIET
        ERROR_QUIET
    )
    if (found EQUAL 0)
        set(Pip3_FOUND TRUE CACHE INTERNAL "If pip3 was found on the system")
        set(Pip3_Python "${loc}" CACHE INTERNAL "The path to python used for pip3")
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Pip3 REQUIRED_VARS Pip3_Python)
