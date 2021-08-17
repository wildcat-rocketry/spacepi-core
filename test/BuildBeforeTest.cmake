execute_process(COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}" --target spacepi-test RESULT_VARIABLE ret)
if (NOT ret EQUAL 0)
    message(FATAL_ERROR)
endif()
