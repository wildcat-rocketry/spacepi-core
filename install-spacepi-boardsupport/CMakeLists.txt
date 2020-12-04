add_executable(install-spacepi-boardsupport
    src/main.cpp
)

target_link_libraries(install-spacepi-boardsupport
    spacepi
)
target_include_directories(install-spacepi-boardsupport PUBLIC include)

target_precompile_headers(install-spacepi-boardsupport REUSE_FROM spacepi_pch)
