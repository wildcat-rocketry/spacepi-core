find_package(Protobuf REQUIRED)

spacepi_module(
    CORE LIBRARY NAME spacepi-protoc
    SOURCES
        src/CodeGenerator.cpp
        src/CodeStream.cpp
        src/CodeTemplate.cpp
    INCLUDE include
    LINK
        protobuf::libprotobuf
        protobuf::libprotoc
)
