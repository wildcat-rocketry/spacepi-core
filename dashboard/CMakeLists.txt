cmake_minimum_required(VERSION 3.0)
project(spacepi-dashboard)
include(../cmake/SpacePiSetup.cmake)

include(SpacePiDesktopOnly)
spacepi_desktop_only()

include(SpacePiDashboard)

file(RELATIVE_PATH relProj "${CMAKE_BINARY_DIR}/_dashboard" "${CMAKE_CURRENT_SOURCE_DIR}")
string(REPLACE "/" "\\" relProj "${relProj}")

file(
    GENERATE OUTPUT "${CMAKE_BINARY_DIR}/_dashboard/SpacePi Dashboard.sln"
    INPUT "${CMAKE_CURRENT_SOURCE_DIR}/SpacePi Dashboard.sln.in"
)

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/_dashboard/.nuget")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/nuget.config.in"
    "${CMAKE_BINARY_DIR}/_dashboard/nuget.config"
)

add_subdirectory(SpacePi.Dashboard.WPFMain)
add_subdirectory(SpacePi.Dashboard.Analyzer)
add_subdirectory(SpacePi.Dashboard.API)
add_subdirectory(SpacePi.Dashboard.Build)
add_subdirectory(SpacePi.Dashboard.BuildConfig)
add_subdirectory(SpacePi.Dashboard.BuildTool)
add_subdirectory(SpacePi.Dashboard.Core)
add_subdirectory(SpacePi.Dashboard.UWP)
add_subdirectory(SpacePi.Dashboard.WPF)
