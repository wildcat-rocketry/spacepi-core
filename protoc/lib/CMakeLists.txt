spacepi_dependencies(
    LIBPROTOBUF
    LIBPROTOC
)

spacepi_aux_library(
    spacepi-protoc SHARED
    SOURCES
        src/CodeGenerator.cpp
        src/CodeStream.cpp
        src/CodeTemplate.cpp
    INCLUDE include
    LINK
        LIBPROTOC
        LIBPROTOBUF
    NO_INSTALL
)
