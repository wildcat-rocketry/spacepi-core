include(SpacePiDesktopOnly)
spacepi_desktop_only()

include(SpacePiDashboard)

file(
    GENERATE OUTPUT "${CMAKE_BINARY_DIR}/_dashboard/SpacePi Dashboard.sln"
    INPUT "${CMAKE_CURRENT_SOURCE_DIR}/SpacePi Dashboard.sln.in"
)

#add_subdirectory(SpacePi.Dashboard.WPF)
add_subdirectory(SpacePi.Dashboard.Analyzer)
add_subdirectory(SpacePi.Dashboard.Analyzer.API)
add_subdirectory(SpacePi.Dashboard.API)
#add_subdirectory(SpacePi.Dashboard.Core)
#add_subdirectory(SpacePi.Dashboard.Core.WPF)
#add_subdirectory(SpacePi.Dashboard.Test)
#add_subdirectory(SpacePi.Dashboard.UWP)
