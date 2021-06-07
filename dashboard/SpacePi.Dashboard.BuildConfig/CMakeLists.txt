include(SpacePiDashboard)
spacepi_dashboard_plugin(
    "${CMAKE_BINARY_DIR}/_dashboard/SpacePi.Dashboard.BuildConfig/SpacePi.Dashboard.BuildConfig.csproj" "{A1A2374B-DA09-49CB-AAC5-6D7F9DFC74A3}"
    NOT_PLUGIN
    TYPE "{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}"
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/SpacePi.Dashboard.BuildConfig.csproj.in"
    "${CMAKE_BINARY_DIR}/_dashboard/SpacePi.Dashboard.BuildConfig/SpacePi.Dashboard.BuildConfig.csproj"
)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/BuildConfig.in.cs"
    "${CMAKE_BINARY_DIR}/_dashboard/SpacePi.Dashboard.BuildConfig/BuildConfig.cs.in"
)
file(
    GENERATE OUTPUT "${CMAKE_BINARY_DIR}/_dashboard/SpacePi.Dashboard.BuildConfig/BuildConfig.cs"
    INPUT "${CMAKE_BINARY_DIR}/_dashboard/SpacePi.Dashboard.BuildConfig/BuildConfig.cs.in"
)
