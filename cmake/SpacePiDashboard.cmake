# spacepi_dashboard_plugin(
#     <csproj file> <guid>
#     [NOT_PLUGIN]
#     [TYPE <guid>]
#     [DEPENDS <guid> [<guid> ...]]
# )
function (spacepi_dashboard_plugin SPACEPI_DASHBOARD_PROJ SPACEPI_DASHBOARD_GUID)
    cmake_parse_arguments(SPACEPI_DASHBOARD "NOT_PLUGIN" "TYPE" "DEPENDS" ${ARGN})

    if (NOT SPACEPI_DASHBOARD_TYPE)
        set(SPACEPI_DASHBOARD_TYPE "{9A19103F-16F7-4668-BE54-9A1E7A4F7556}")
    endif()

    if (NOT SPACEPI_DASHBOARD_NOT_PLUGIN)
        list(APPEND SPACEPI_DASHBOARD_DEPENDS "{0F502909-B2ED-419D-96A1-DFA6D482C94D}")
    endif()

    get_target_property(slnProjs dashboard-metadata SPACEPI_DASHBOARD_SLN_PROJS)
    get_target_property(slnCfgs dashboard-metadata SPACEPI_DASHBOARD_SLN_CFGS)
    get_target_property(csprojRefs dashboard-metadata SPACEPI_DASHBOARD_CSPROJ_REFS)

    get_filename_component(SPACEPI_DASHBOARD_PROJ "${SPACEPI_DASHBOARD_PROJ}" ABSOLUTE)
    get_filename_component(projName "${SPACEPI_DASHBOARD_PROJ}" NAME)
    string(REGEX REPLACE "\\.csproj$" "" projName "${projName}")
    file(RELATIVE_PATH relProj "${CMAKE_BINARY_DIR}/_dashboard" "${SPACEPI_DASHBOARD_PROJ}")
    string(REPLACE "/" "\\" relProj "${relProj}")

    if (SPACEPI_DASHBOARD_DEPENDS)
        string(APPEND slnProjs
"Project(\"${SPACEPI_DASHBOARD_TYPE}\") = \"${projName}\", \"${relProj}\", \"${SPACEPI_DASHBOARD_GUID}\"
	ProjectSection(ProjectDependencies) = postProject
")
        foreach (dep IN LISTS SPACEPI_DASHBOARD_DEPENDS)
            string(APPEND slnProjs
"		${dep} = ${dep}
")
        endforeach()
        string(APPEND slnProjs
"	EndProjectSection
EndProject
")
    else()
        string(APPEND slnProjs
"Project(\"${SPACEPI_DASHBOARD_TYPE}\") = \"${projName}\", \"${relProj}\", \"${SPACEPI_DASHBOARD_GUID}\"
EndProject
")
    endif()
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
    if (NOT SPACEPI_DASHBOARD_NOT_PLUGIN)
        string(APPEND csprojRefs
"        <ProjectReference Include=\"..\\${relProj}\" />
")
    endif()

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
