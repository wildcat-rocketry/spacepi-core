include(SpacePiDashboard)
spacepi_dashboard_plugin(
    "${CMAKE_BINARY_DIR}/_dashboard/SpacePi.Dashboard.UWP/SpacePi.Dashboard.UWP.wapproj" "{3896F325-27BF-445F-90AD-4E929FA7D439}"
    NOT_PLUGIN
    TYPE "{C7167F0D-BC9F-4E6E-AFE1-012C56B48DB5}"
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/SpacePi.Dashboard.UWP.wapproj.in"
    "${CMAKE_BINARY_DIR}/_dashboard/SpacePi.Dashboard.UWP/SpacePi.Dashboard.UWP.wapproj"
)
