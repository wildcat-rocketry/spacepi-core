find_package(Protobuf REQUIRED)

spacepi_module(
    CORE MODULE NAME protoc-gen-spacepi-csharp
    SOURCES
        src/main.cpp
        src/CSharpGen.cpp
    INCLUDE include
    LINK
        spacepi-protoc
        protobuf::libprotobuf
        protobuf::libprotoc
)
