if (CMAKE_VERSION VERSION_LESS "3.16")
    function (target_precompile_headers)
    endfunction()
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-psabi)
endif()
