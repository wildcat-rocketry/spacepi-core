cmake_minimum_required(VERSION 3.11)
project(spacepi-core)
include(cmake/CoreRepo.cmake)

spacepi_message_library(core-messages ${SPACEPI_CORE_SOURCE_DIR}/include
    spacepi/messaging/EncapsulatedMessage.proto
    spacepi/messaging/HelloMessage.proto
    spacepi/messaging/MessageID.proto
    spacepi/messaging/SubscribeRequest.proto
)

find_package(Boost
    REQUIRED COMPONENTS
    # asio
    context
    exception
    fiber
    # format
    program_options
    stacktrace_backtrace
    system
)
find_package(Protobuf)

add_library(spacepi SHARED
    src/concurrent/ThreadPool.cpp
    src/log/ConsoleTarget.cpp
    src/log/Entry.cpp
    src/log/Logger.cpp
    src/log/LogLevel.cpp
    src/log/LogManager.cpp
    src/log/LogStream.cpp
    src/messaging/network/MessagingSocket.cpp
    src/messaging/network/NetworkThread.cpp
    src/messaging/network/SocketWrapper.cpp
    src/messaging/Connection.cpp
    src/messaging/Subscription.cpp
    src/util/CommandConfigurable.cpp
    src/util/Exception.cpp
)
target_include_directories(spacepi PUBLIC ${SPACEPI_CORE_SOURCE_DIR}/include)
target_link_libraries(spacepi
    Boost::context
    Boost::exception
    Boost::fiber
    Boost::program_options
    Boost::stacktrace_backtrace
    Boost::system
    ${Protobuf_LIBRARIES}
    core-messages
)
install(TARGETS spacepi LIBRARY DESTINATION lib)

add_subdirectory(dashboard)
add_subdirectory(examples EXCLUDE_FROM_ALL)
add_subdirectory(router)
