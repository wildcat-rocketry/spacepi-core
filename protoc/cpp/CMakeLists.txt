find_package(Protobuf REQUIRED)

spacepi_module(
    CORE MODULE NAME protoc-gen-spacepi-cpp
    SOURCES
        src/main.cpp
        src/CPPGen.cpp
        src/HPPGen.cpp
    INCLUDE include
    LINK
        spacepi-protoc
        protobuf::libprotobuf
        protobuf::libprotoc
)
