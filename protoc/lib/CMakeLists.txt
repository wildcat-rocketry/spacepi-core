find_package(Protobuf REQUIRED)

add_library(spacepi-protoc STATIC
    src/CodeGenerator.cpp
    src/CodeStream.cpp
    src/CodeTemplate.cpp
)

target_include_directories(spacepi-protoc PUBLIC
    include
)

target_link_libraries(spacepi-protoc
    protobuf::libprotoc
    protobuf::libprotobuf
)
