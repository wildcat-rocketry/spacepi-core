configure_file("SpacePi.Format.sln.in" "${CMAKE_CURRENT_BINARY_DIR}/SpacePi.Format.sln")
configure_file("../.editorconfig" "${CMAKE_CURRENT_BINARY_DIR}/.editorconfig")
configure_file("../dotnet-format/NuGet.config" "${CMAKE_CURRENT_BINARY_DIR}/NuGet.config")
configure_file("SpacePi.Format.targets.in" "${CMAKE_CURRENT_BINARY_DIR}/SpacePi.Format.targets.in")
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/SpacePi.Format.targets" INPUT "${CMAKE_CURRENT_BINARY_DIR}/SpacePi.Format.targets.in")

find_package(DotNet REQUIRED)

add_custom_target(format-build
    COMMAND ${DotNet} build SpacePi.Format.sln
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

add_custom_target(format-impl
    COMMAND ${DotNet} "${CMAKE_CURRENT_BINARY_DIR}/SpacePi.Format/bin/netcoreapp2.1/SpacePi.Format.dll"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    DEPENDS format-build
)
add_dependencies(format format-impl)

add_custom_target(format-check-impl
    COMMAND ${DotNet} "${CMAKE_CURRENT_BINARY_DIR}/SpacePi.Format/bin/netcoreapp2.1/SpacePi.Format.dll" -n
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    DEPENDS format-build
)
add_dependencies(format-check format-check-impl)
