cmake_minimum_required(VERSION 3.0)
project(spacepi-core)
include(cmake/CoreRepo.cmake)

spacepi_message_library(core-messages ${SPACEPI_CORE_SOURCE_DIR}/include
    spacepi/messaging/subscribe.proto
    spacepi/messaging/message-id.proto
)

find_package(Boost
    REQUIRED COMPONENTS
    # asio
    exception
    fiber
    # format
    program_options
    stacktrace_backtrace
    system # for asio
)

add_library(spacepi SHARED
    src/concurrent/ThreadPool.cpp
    src/messaging/Connection.cpp
    src/messaging/Subscription.cpp
    src/util/CommandConfigurable.cpp
    src/util/Exception.cpp
)
target_include_directories(spacepi PUBLIC ${SPACEPI_CORE_SOURCE_DIR}/include)
target_link_libraries(spacepi
    Boost::exception
    Boost::fiber
    Boost::program_options
    Boost::stacktrace_backtrace
    Boost::system
    core-messages
)

add_subdirectory(dashboard)
add_subdirectory(router)
