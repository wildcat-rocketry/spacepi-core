find_package(Protobuf REQUIRED)

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/include/spacepi/protoc/transient.pb.cc"
           "${CMAKE_CURRENT_BINARY_DIR}/include/spacepi/protoc/transient.pb.h"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/include"
    COMMAND protobuf::protoc -I "${CMAKE_CURRENT_SOURCE_DIR}/include" "--cpp_out=${CMAKE_CURRENT_BINARY_DIR}/include" spacepi/protoc/transient.proto
    MAIN_DEPENDENCY include/spacepi/protoc/transient.proto
    COMMENT "Running C++ protoc on include/spacepi/protoc/transient.proto"
    VERBATIM
)

add_executable(protoc-gen-spacepi-csharp
    src/main.cpp
    src/CSharpGen.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/include/spacepi/protoc/transient.pb.cc
)

target_include_directories(protoc-gen-spacepi-csharp PUBLIC
    include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_link_libraries(protoc-gen-spacepi-csharp
    spacepi-protoc
    protobuf::libprotoc
    protobuf::libprotobuf
)

set_target_properties(
    protoc-gen-spacepi-csharp
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
foreach(config IN LISTS CMAKE_CONFIGURATION_TYPES)
    string(TOUPPER "${config}" upperConfig)
    set_target_properties(
        protoc-gen-spacepi-csharp
        PROPERTIES
            "RUNTIME_OUTPUT_DIRECTORY_${upperConfig}" "${CMAKE_CURRENT_BINARY_DIR}"
    )
endforeach()
