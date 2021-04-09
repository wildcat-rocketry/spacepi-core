spacepi_dependencies(
    LIBPROTOBUF
    LIBPROTOC
)

spacepi_aux_program(
    protoc-gen-spacepi-csharp
    SOURCES
        src/main.cpp
        src/CSharpGen.cpp
    INCLUDE include
    LINK
        spacepi-protoc
        LIBPROTOC
        LIBPROTOBUF
    NO_INSTALL
)
