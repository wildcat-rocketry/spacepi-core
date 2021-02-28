spacepi_aux_program(
    install-spacepi-boardsupport
    SOURCES
        src/main.cpp
    INCLUDE include
    LINK spacepi
    PRECOMPILE_FROM spacepi_pch
)
