# spacepi_dashboard_plugin(<csproj file> <guid>)
function (spacepi_dashboard_plugin SPACEPI_DASHBOARD_PROJ SPACEPI_DASHBOARD_GUID)
    get_target_property(slnProjs dashboard-metadata SPACEPI_DASHBOARD_SLN_PROJS)
    get_target_property(slnCfgs dashboard-metadata SPACEPI_DASHBOARD_SLN_CFGS)
    get_target_property(csprojRefs dashboard-metadata SPACEPI_DASHBOARD_CSPROJ_REFS)

    get_filename_component(projName "${SPACEPI_DASHBOARD_PROJ}" NAME)
    string(REGEX REPLACE "\\.csproj$" "" projName "${projName}")
    file(RELATIVE_PATH relProj "${CMAKE_BINARY_DIR}/_dashboard" "${CMAKE_CURRENT_SOURCE_DIR}/${SPACEPI_DASHBOARD_PROJ}")
    string(REPLACE "/" "\\" relProj "${relProj}")

    string(APPEND slnProjs
"Project(\"{9A19103F-16F7-4668-BE54-9A1E7A4F7556}\") = \"${projName}\", \"${relProj}\", \"${SPACEPI_DASHBOARD_GUID}\"
	ProjectSection(ProjectDependencies) = postProject
		{0F502909-B2ED-419D-96A1-DFA6D482C94D} = {0F502909-B2ED-419D-96A1-DFA6D482C94D}
	EndProjectSection
EndProject
")
    string(APPEND slnCfgs
"		${SPACEPI_DASHBOARD_GUID}.Debug|Any CPU.ActiveCfg = Debug|Any CPU
		${SPACEPI_DASHBOARD_GUID}.Debug|Any CPU.Build.0 = Debug|Any CPU
		${SPACEPI_DASHBOARD_GUID}.Debug|ARM.ActiveCfg = Debug|Any CPU
		${SPACEPI_DASHBOARD_GUID}.Debug|ARM.Build.0 = Debug|Any CPU
		${SPACEPI_DASHBOARD_GUID}.Debug|ARM64.ActiveCfg = Debug|Any CPU
		${SPACEPI_DASHBOARD_GUID}.Debug|ARM64.Build.0 = Debug|Any CPU
		${SPACEPI_DASHBOARD_GUID}.Debug|x64.ActiveCfg = Debug|Any CPU
		${SPACEPI_DASHBOARD_GUID}.Debug|x64.Build.0 = Debug|Any CPU
		${SPACEPI_DASHBOARD_GUID}.Debug|x86.ActiveCfg = Debug|Any CPU
		${SPACEPI_DASHBOARD_GUID}.Debug|x86.Build.0 = Debug|Any CPU
		${SPACEPI_DASHBOARD_GUID}.Release|Any CPU.ActiveCfg = Release|Any CPU
		${SPACEPI_DASHBOARD_GUID}.Release|Any CPU.Build.0 = Release|Any CPU
		${SPACEPI_DASHBOARD_GUID}.Release|ARM.ActiveCfg = Release|Any CPU
		${SPACEPI_DASHBOARD_GUID}.Release|ARM.Build.0 = Release|Any CPU
		${SPACEPI_DASHBOARD_GUID}.Release|ARM64.ActiveCfg = Release|Any CPU
		${SPACEPI_DASHBOARD_GUID}.Release|ARM64.Build.0 = Release|Any CPU
		${SPACEPI_DASHBOARD_GUID}.Release|x64.ActiveCfg = Release|Any CPU
		${SPACEPI_DASHBOARD_GUID}.Release|x64.Build.0 = Release|Any CPU
		${SPACEPI_DASHBOARD_GUID}.Release|x86.ActiveCfg = Release|Any CPU
		${SPACEPI_DASHBOARD_GUID}.Release|x86.Build.0 = Release|Any CPU
")
    string(APPEND csprojRefs
"        <ProjectReference Include=\"..\\${relProj}\" />
")

    set_target_properties(
        dashboard-metadata PROPERTIES
        SPACEPI_DASHBOARD_SLN_PROJS "${slnProjs}"
        SPACEPI_DASHBOARD_SLN_CFGS "${slnCfgs}"
        SPACEPI_DASHBOARD_CSPROJ_REFS "${csprojRefs}"
    )
endfunction()

include(SpacePiOnce)
spacepi_once(SPACEPI_DASHBOARD)

define_property(TARGET PROPERTY SPACEPI_DASHBOARD_SLN_PROJS BRIEF_DOCS "." FULL_DOCS ".")
define_property(TARGET PROPERTY SPACEPI_DASHBOARD_SLN_CFGS BRIEF_DOCS "." FULL_DOCS ".")
define_property(TARGET PROPERTY SPACEPI_DASHBOARD_CSPROJ_REFS BRIEF_DOCS "." FULL_DOCS ".")

add_custom_target(dashboard-metadata)

set_target_properties(
    dashboard-metadata PROPERTIES
    SPACEPI_DASHBOARD_SLN_PROJS ""
    SPACEPI_DASHBOARD_SLN_CFGS ""
    SPACEPI_DASHBOARD_CSPROJ_REFS ""
)
