cmake_minimum_required(VERSION 3.11)
project(spacepi-core)
include(cmake/CoreRepo.cmake)

spacepi_message_library(core-messages ${SPACEPI_CORE_SOURCE_DIR}/include
    spacepi/messaging/EncapsulatedMessage.proto
    spacepi/messaging/HelloMessage.proto
    spacepi/messaging/MessageID.proto
    spacepi/messaging/SubscribeRequest.proto
    spacepi/messaging/SubscriptionID.proto
)

find_package(Boost
    REQUIRED COMPONENTS
    # asio
    context
    exception
    fiber
    # format
    stacktrace_backtrace
    system
)
find_package(Protobuf)

add_library(spacepi SHARED
    src/concurrent/ThreadPool.cpp
    src/log/ConsoleTarget.cpp
    src/log/Entry.cpp
    src/log/LogFilter.cpp
    src/log/LogFilterCommand.cpp
    src/log/Logger.cpp
    src/log/LogLevel.cpp
    src/log/LogManager.cpp
    src/log/LogStream.cpp
    src/messaging/network/MessagingCallback.cpp
    src/messaging/network/MessagingSocket.cpp
    src/messaging/network/NetworkThread.cpp
    src/messaging/network/SocketWrapper.cpp
    src/messaging/network/SubscriptionID.cpp
    src/messaging/Connection.cpp
    src/messaging/Subscription.cpp
    src/resource/ADC.cpp
    src/resource/BusTransaction.cpp
    src/resource/DigitalIO.cpp
    src/resource/I2C.cpp
    src/resource/Memory.cpp
    src/resource/Processor.cpp
    src/resource/SPI.cpp
    src/resource/UART.cpp
    src/util/Command.cpp
    src/util/CommandConfigurable.cpp
    src/util/CommandInternals.cpp
    src/util/Exception.cpp
)
target_include_directories(spacepi PUBLIC ${SPACEPI_CORE_SOURCE_DIR}/include)
target_link_libraries(spacepi
    Boost::context
    Boost::exception
    Boost::fiber
    Boost::stacktrace_backtrace
    Boost::system
    ${Protobuf_LIBRARIES}
    core-messages
)
target_precompile_headers(spacepi PUBLIC
    <SpacePi.hpp>
)
set_target_properties(spacepi PROPERTIES DEFINE_SYMBOL "")
install(TARGETS spacepi LIBRARY DESTINATION lib)

add_executable(spacepi_pch src/PCH.cpp)
target_link_libraries(spacepi_pch spacepi)
target_precompile_headers(spacepi_pch PUBLIC
    <SpacePi.hpp>
)

find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_ENABLE_PREPROCESSING YES)
    set(DOXYGEN_PREDEFINED DOXYGEN_SHOULD_SKIP_THIS)
    set(DOXYGEN_EXCLUDE_SYMBOLS "*::detail::*;std::*;boost::*")
    set(DOXYGEN_QUIET YES)
    doxygen_add_docs(spacepi-docs
        include/spacepi/concurrent/RWMutex.hpp
        include/spacepi/concurrent/Semaphore.hpp
        include/spacepi/concurrent/Sleep.hpp
        include/spacepi/concurrent/ThreadPool.hpp
        include/spacepi/log/AutoLog.hpp
        include/spacepi/log/ConsoleTarget.hpp
        include/spacepi/log/Entry.hpp
        include/spacepi/log/LogFilter.hpp
        include/spacepi/log/LogFilterCommand.hpp
        include/spacepi/log/Logger.hpp
        include/spacepi/log/LogLevel.hpp
        include/spacepi/log/LogManager.hpp
        include/spacepi/log/LogStream.hpp
        include/spacepi/log/LogTarget.hpp
        include/spacepi/messaging/network/MessagingCallback.hpp
        include/spacepi/messaging/network/MessagingSocket.hpp
        include/spacepi/messaging/network/NetworkThread.hpp
        include/spacepi/messaging/network/SocketWrapper.hpp
        include/spacepi/messaging/network/SocketWrapperCallback.hpp
        include/spacepi/messaging/network/SubscriptionID.hpp
        include/spacepi/messaging/Connection.hpp
        include/spacepi/messaging/Network.hpp
        include/spacepi/messaging/Subscription.hpp
        include/spacepi/resource/ADC.hpp
        include/spacepi/resource/BusTransaction.hpp
        include/spacepi/resource/DigitalIO.hpp
        include/spacepi/resource/I2C.hpp
        include/spacepi/resource/Memory.hpp
        include/spacepi/resource/Processor.hpp
        include/spacepi/resource/ResourceFactory.hpp
        include/spacepi/resource/ResourcePtr.hpp
        include/spacepi/resource/SPI.hpp
        include/spacepi/resource/UART.hpp
        include/spacepi/util/Command.hpp
        include/spacepi/util/CommandConfigurable.hpp
        include/spacepi/util/CommandInternals.hpp
        include/spacepi/util/Exception.hpp
        include/spacepi/util/SharedOrRef.hpp
        include/spacepi/util/TemporalQueue.hpp
        include/spacepi/util/Trie.hpp
        include/spacepi/util/WeakOrRef.hpp
        include/spacepi/Concurrent.hpp
        include/spacepi/Log.hpp
        include/spacepi/Messaging.hpp
        include/spacepi/Resource.hpp
        include/spacepi/Util.hpp
        include/SpacePi.hpp
        ALL USE_STAMP_FILE
    )
endif()

add_subdirectory(dashboard)
add_subdirectory(examples EXCLUDE_FROM_ALL)
add_subdirectory(router)
