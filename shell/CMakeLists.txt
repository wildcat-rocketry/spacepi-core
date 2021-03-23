spacepi_aux_program(
    spacepi-shell
    SOURCES
        src/main.c
    NO_INSTALL
)

add_dependencies(installable spacepi-shell)

install(
    TARGETS spacepi-shell
    RUNTIME DESTINATION bin
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE SETUID
)
