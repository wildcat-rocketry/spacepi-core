# spacepi_dashboard_plugin(
#     <name> <guid>
#     [NOT_PLUGIN]
#     [TYPE <guid>]
#     [PROJ_TYPE <tproj>]
#     [DEPENDS <guid> [<guid> ...]]
#     [CONFIG_FILE <file> [<file> ...]]
# )
function (spacepi_dashboard_plugin SPACEPI_DASHBOARD_PROJ SPACEPI_DASHBOARD_GUID)
    cmake_parse_arguments(SPACEPI_DASHBOARD "NOT_PLUGIN" "TYPE;PROJ_TYPE" "DEPENDS;CONFIG_FILE" ${ARGN})

    if (NOT SPACEPI_DASHBOARD_TYPE)
        set(SPACEPI_DASHBOARD_TYPE "{9A19103F-16F7-4668-BE54-9A1E7A4F7556}")
    endif()

    if (NOT SPACEPI_DASHBOARD_PROJ_TYPE)
        set(SPACEPI_DASHBOARD_PROJ_TYPE "csproj")
    endif()

    if (NOT SPACEPI_DASHBOARD_NOT_PLUGIN)
        list(APPEND SPACEPI_DASHBOARD_DEPENDS "{0F502909-B2ED-419D-96A1-DFA6D482C94D}")
    endif()

    get_target_property(slnProjs dashboard-metadata SPACEPI_DASHBOARD_SLN_PROJS)
    get_target_property(slnCfgs dashboard-metadata SPACEPI_DASHBOARD_SLN_CFGS)
    get_target_property(csprojRefs dashboard-metadata SPACEPI_DASHBOARD_CSPROJ_REFS)

    file(RELATIVE_PATH relProjBuild "${CMAKE_BINARY_DIR}/_dashboard" "${CMAKE_CURRENT_SOURCE_DIR}/${SPACEPI_DASHBOARD_PROJ}.${SPACEPI_DASHBOARD_PROJ_TYPE}")
    string(REPLACE "/" "\\" relProjBuild "${relProjBuild}")
    get_property(spacePiCore GLOBAL PROPERTY SPACEPI_CORE_DIR)
    file(RELATIVE_PATH relProjSrc "${spacePiCore}/dashboard" "${CMAKE_CURRENT_SOURCE_DIR}/${SPACEPI_DASHBOARD_PROJ}.${SPACEPI_DASHBOARD_PROJ_TYPE}")
    string(REPLACE "/" "\\" relProjSrc "${relProjSrc}")

    foreach (file IN LISTS SPACEPI_DASHBOARD_CONFIG_FILE)
        configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${file}.in" "${CMAKE_BINARY_DIR}/_dashboard/${SPACEPI_DASHBOARD_PROJ}/${file}.in")
        file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/_dashboard/${SPACEPI_DASHBOARD_PROJ}/${file}" INPUT "${CMAKE_BINARY_DIR}/_dashboard/${SPACEPI_DASHBOARD_PROJ}/${file}.in")
    endforeach()

    if (SPACEPI_DASHBOARD_DEPENDS)
        string(APPEND slnProjs
"Project(\"${SPACEPI_DASHBOARD_TYPE}\") = \"${SPACEPI_DASHBOARD_PROJ}\", \"${relProjBuild}\", \"${SPACEPI_DASHBOARD_GUID}\"
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
"Project(\"${SPACEPI_DASHBOARD_TYPE}\") = \"${SPACEPI_DASHBOARD_PROJ}\", \"${relProjBuild}\", \"${SPACEPI_DASHBOARD_GUID}\"
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
"        <ProjectReference Include=\"..\\${relProjSrc}\" />
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
