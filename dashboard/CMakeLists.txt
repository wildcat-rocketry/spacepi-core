cmake_minimum_required(VERSION 3.0)
project(spacepi-dashboard)
include(../cmake/SpacePiSetup.cmake)

include(SpacePiDesktopOnly)
spacepi_desktop_only()

include(SpacePiDashboard)

file(RELATIVE_PATH relProj "${CMAKE_BINARY_DIR}/_dashboard" "${CMAKE_CURRENT_SOURCE_DIR}")
string(REPLACE "/" "\\" relProj "${relProj}")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/SpacePi Dashboard.sln.in"
    "${CMAKE_BINARY_DIR}/_dashboard/SpacePi Dashboard.sln.in"
)
file(
    GENERATE OUTPUT "${CMAKE_BINARY_DIR}/_dashboard/SpacePi Dashboard.sln"
    INPUT "${CMAKE_BINARY_DIR}/_dashboard/SpacePi Dashboard.sln.in"
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/FFAero.SpacePi.Dashboard.WPFMain/FFAero.SpacePi.Dashboard.WPFMain.csproj.in"
    "${CMAKE_BINARY_DIR}/_dashboard/FFAero.SpacePi.Dashboard.WPFMain/FFAero.SpacePi.Dashboard.WPFMain.csproj.in"
)
file(
    GENERATE OUTPUT "${CMAKE_BINARY_DIR}/_dashboard/FFAero.SpacePi.Dashboard.WPFMain/FFAero.SpacePi.Dashboard.WPFMain.csproj"
    INPUT "${CMAKE_BINARY_DIR}/_dashboard/FFAero.SpacePi.Dashboard.WPFMain/FFAero.SpacePi.Dashboard.WPFMain.csproj.in"
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/FFAero.SpacePi.Dashboard.UWP/FFAero.SpacePi.Dashboard.UWP.wapproj.in"
    "${CMAKE_BINARY_DIR}/_dashboard/FFAero.SpacePi.Dashboard.UWP/FFAero.SpacePi.Dashboard.UWP.wapproj"
)

add_subdirectory(FFAero.SpacePi.Dashboard.Core)
