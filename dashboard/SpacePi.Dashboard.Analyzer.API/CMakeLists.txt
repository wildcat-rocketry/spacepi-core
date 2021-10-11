include(SpacePiDashboard)
find_package(Protobuf REQUIRED)
spacepi_dashboard_plugin(
    "SpacePi.Dashboard.Analyzer.API" "{A49A4867-6DF5-4441-8CC0-2827E7CF83C6}"
    NOT_PLUGIN
    CONFIG_FILE "BuildConfig.cs"
)
