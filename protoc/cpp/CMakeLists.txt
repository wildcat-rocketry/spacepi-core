spacepi_dependencies(
    LIBPROTOBUF
    LIBPROTOC
)

spacepi_aux_program(
    protoc-gen-spacepi-cpp
    SOURCES
        src/main.cpp
        src/CPPGen.cpp
        src/HPPGen.cpp
    INCLUDE include
    LINK
        spacepi-protoc
        LIBPROTOC
        LIBPROTOBUF
    NO_INSTALL
)
