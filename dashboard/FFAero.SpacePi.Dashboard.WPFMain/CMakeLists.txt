include(SpacePiDashboard)
spacepi_dashboard_plugin(
    "${CMAKE_BINARY_DIR}/_dashboard/FFAero.SpacePi.Dashboard.WPFMain/FFAero.SpacePi.Dashboard.WPFMain.csproj" "{2E950B2A-2646-4301-A4A9-889A3FCAA2B1}"
    NOT_PLUGIN
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/FFAero.SpacePi.Dashboard.WPFMain.csproj.in"
    "${CMAKE_BINARY_DIR}/_dashboard/FFAero.SpacePi.Dashboard.WPFMain/FFAero.SpacePi.Dashboard.WPFMain.csproj.in"
)
file(
    GENERATE OUTPUT "${CMAKE_BINARY_DIR}/_dashboard/FFAero.SpacePi.Dashboard.WPFMain/FFAero.SpacePi.Dashboard.WPFMain.csproj"
    INPUT "${CMAKE_BINARY_DIR}/_dashboard/FFAero.SpacePi.Dashboard.WPFMain/FFAero.SpacePi.Dashboard.WPFMain.csproj.in"
)
