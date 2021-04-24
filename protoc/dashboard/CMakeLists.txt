find_package(Protobuf REQUIRED)

spacepi_module(
    CORE MODULE NAME protoc-gen-spacepi-dashboard
    SOURCES
        src/main.cpp
    LINK
        protobuf::libprotoc
        protobuf::libprotobuf
)
