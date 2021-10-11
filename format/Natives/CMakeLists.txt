file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/Clang.cs" INPUT "Clang.cs.in")

llvm_add_library(
    SpacePi.Format.Natives
        src/Clang.cpp
    SHARED
    LINK_COMPONENTS
        support
    LINK_LIBS
        clangBasic
        clangFormat
        clangRewrite
        clangToolingCore
)

target_include_directories(SpacePi.Format.Natives PUBLIC
    include
    "${CMAKE_CURRENT_BINARY_DIR}/../clang/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../llvm/clang/include"
)

set_target_properties(
    SpacePi.Format.Natives
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
foreach(config IN LISTS CMAKE_CONFIGURATION_TYPES)
    string(TOUPPER "${config}" upperConfig)
    set_target_properties(
        SpacePi.Format.Natives
        PROPERTIES
            "RUNTIME_OUTPUT_DIRECTORY_${upperConfig}" "${CMAKE_CURRENT_BINARY_DIR}"
    )
endforeach()
