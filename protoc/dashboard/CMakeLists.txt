find_package(Protobuf REQUIRED)

add_executable(protoc-gen-spacepi-dashboard
    src/main.cpp
)

target_include_directories(protoc-gen-spacepi-dashboard PUBLIC
    include
)

target_link_libraries(protoc-gen-spacepi-dashboard
    protobuf::libprotobuf
    protobuf::libprotoc
)
