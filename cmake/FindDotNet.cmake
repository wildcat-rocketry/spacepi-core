if (DotNet)
    return()
endif()

find_program(DotNet dotnet)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DotNet REQUIRED_VARS DotNet)
