find_package(GTest REQUIRED)
add_executable(spacepi-test
    src/concurrent/Lock.cpp
    src/concurrent/Mutex.cpp
    src/container/Function.cpp
    src/container/Property.cpp
    src/container/SmartPtr.cpp
    src/dtc/includer.cpp
    src/dtc/main.cpp
    src/Parameter.cpp
    src/platform/console.cpp
    src/platform/Ints.cpp
    src/platform/testAndSet.cpp
    src/test/dtc/MockDiagnosticReporter.cpp
    src/test/dtc/MockIncludeFinder.cpp
    src/test/dtc/MockParser.cpp
    src/test/dtc/StubLexer.cpp
    src/test/dtc/StubTokenizer.cpp
    src/test/dtc/SyntaxTreeBuilder.cpp
    src/test/TypeChecker.cpp
    src/TypeUtil.cpp
)
target_include_directories(spacepi-test PUBLIC
    include
)
target_link_libraries(spacepi-test
    spacepi-pch
    spacepi-platform
    spacepi-libdtc
    GTest::gtest
    GTest::gtest_main
)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/BuildBeforeTest.cmake" "${CMAKE_CURRENT_BINARY_DIR}/BuildBeforeTest.cmake")
set_property(DIRECTORY APPEND PROPERTY TEST_INCLUDE_FILES "${CMAKE_CURRENT_BINARY_DIR}/BuildBeforeTest.cmake")

include(GoogleTest)
gtest_discover_tests(spacepi-test)
