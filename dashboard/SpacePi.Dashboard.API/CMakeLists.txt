include(SpacePiDashboard)
find_package(Protobuf REQUIRED)
spacepi_dashboard_plugin(
    "SpacePi.Dashboard.API" "{0F502909-B2ED-419D-96A1-DFA6D482C94D}"
    NOT_PLUGIN
    CONFIG_FILE "BuildConfig.cs"
)
