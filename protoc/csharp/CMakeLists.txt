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

set_target_properties(
    protoc-gen-spacepi-csharp
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}"
)
foreach(config IN LISTS CMAKE_CONFIGURATION_TYPES)
    string(TOUPPER "${config}" upperConfig)
    set_target_properties(
        protoc-gen-spacepi-csharp
        PROPERTIES
            "RUNTIME_OUTPUT_DIRECTORY_${upperConfig}" "${CMAKE_CURRENT_BINARY_DIR}"
    )
endforeach()
