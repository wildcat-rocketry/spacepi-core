find_package(Protobuf REQUIRED)

add_executable(protoc-gen-spacepi-cpp
    src/main.cpp
    src/CPPGen.cpp
    src/HPPGen.cpp
)

target_include_directories(protoc-gen-spacepi-cpp PUBLIC
    include
)

target_link_libraries(protoc-gen-spacepi-cpp
    spacepi-protoc
    protobuf::libprotobuf
    protobuf::libprotoc
)
