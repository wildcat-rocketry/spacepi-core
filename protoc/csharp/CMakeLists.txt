find_package(Protobuf REQUIRED)

add_executable(protoc-gen-spacepi-csharp
    src/main.cpp
    src/CSharpGen.cpp
)

target_include_directories(protoc-gen-spacepi-csharp PUBLIC
    include
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
