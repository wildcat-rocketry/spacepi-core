find_package(GTest REQUIRED)
add_executable(spacepi-test
    src/platform/ints.cpp
    src/platform/console.cpp
)
target_link_libraries(spacepi-test
    spacepi-pch
    GTest::gtest
    GTest::gtest_main
)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/BuildBeforeTest.cmake" "${CMAKE_CURRENT_BINARY_DIR}/BuildBeforeTest.cmake")
set_property(DIRECTORY APPEND PROPERTY TEST_INCLUDE_FILES "${CMAKE_CURRENT_BINARY_DIR}/BuildBeforeTest.cmake")

include(GoogleTest)
gtest_discover_tests(spacepi-test)
