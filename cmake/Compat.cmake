if (${CMAKE_VERSION} VERSION_LESS "3.16")
    function (target_precompile_headers)
    endfunction()
endif()
