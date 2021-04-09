spacepi_dependencies(
    LIBPROTOBUF
    LIBPROTOC
)

spacepi_aux_program(
    protoc-gen-spacepi-dashboard
    SOURCES
        src/main.cpp
    LINK
        LIBPROTOC
        LIBPROTOBUF
    NO_INSTALL
)
