cmake_minimum_required(VERSION 3.0)
project(spacepi-core)
include(cmake/CoreRepo.cmake)

spacepi_once(CORE_REPO)

spacepi_message_library(
    core-messages include
    SOURCES
        spacepi/messaging/EncapsulatedMessage.proto
        spacepi/messaging/HelloMessage.proto
        spacepi/messaging/MessageID.proto
        spacepi/messaging/SubscribeRequest.proto
        spacepi/messaging/SubscriptionID.proto
    NO_CORE_DEP
)

spacepi_dependencies(
    BOOST_ASIO
    BOOST_CONFIG
    BOOST_CONTEXT
    BOOST_EXCEPTION
    BOOST_FIBER
    BOOST_PROPERTY_TREE
    BOOST_STACKTRACE_BACKTRACE
    BOOST_SYSTEM
    LIBPROTOBUF
)

find_package(Threads REQUIRED)

spacepi_aux_library(
    spacepi SHARED
    SOURCES
        src/concurrent/AsyncInterrupt.cpp
        src/concurrent/ConditionVariable.cpp
        src/concurrent/Interrupt.cpp
        src/concurrent/Sleep.cpp
        src/concurrent/ThreadPool.cpp
        src/concurrent/UniqueConditionVariableLock.cpp
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
        src/messaging/Publisher.cpp
        src/messaging/RawConnection.cpp
        src/messaging/Subscription.cpp
        src/package/Module.cpp
        src/package/Options.cpp
        src/package/PackageConfig.cpp
        src/package/User.cpp
        src/resource/detail/MockADC.cpp
        src/resource/detail/MockDigitalIO.cpp
        src/resource/detail/MockFilesystem.cpp
        src/resource/detail/MockI2C.cpp
        src/resource/detail/MockMemory.cpp
        src/resource/detail/MockProcessor.cpp
        src/resource/detail/MockSPI.cpp
        src/resource/detail/MockUART.cpp
        src/resource/detail/StdFilesystem.cpp
        src/resource/ADC.cpp
        src/resource/BusTransaction.cpp
        src/resource/DigitalIO.cpp
        src/resource/Filesystem.cpp
        src/resource/I2C.cpp
        src/resource/Memory.cpp
        src/resource/Processor.cpp
        src/resource/SPI.cpp
        src/resource/Stream.cpp
        src/resource/UART.cpp
        src/util/Command.cpp
        src/util/CommandConfigurable.cpp
        src/util/CommandInternals.cpp
        src/util/Exception.cpp
    INCLUDE include
    PRECOMPILE <SpacePi.hpp>
    LINK
        BOOST_ASIO
        BOOST_CONFIG
        BOOST_CONTEXT
        BOOST_EXCEPTION
        BOOST_FIBER
        BOOST_PROPERTY_TREE
        BOOST_STACKTRACE_BACKTRACE
        BOOST_SYSTEM
        LIBPROTOBUF
        ${CMAKE_DL_LIBS}
        Threads::Threads
        core-messages
)

set_target_properties(spacepi PROPERTIES DEFINE_SYMBOL "")

spacepi_aux_program(
    spacepi_pch
    SOURCES src/PCH.cpp
    PRECOMPILE <SpacePi.hpp>
    LINK spacepi
    NO_INSTALL
)

spacepi_dependencies(
    OPTIONAL
    DOXYGEN
    DOXYGEN_DOT
    PYTHON3
    PYTHON3_PIP
)

if (TARGET DOXYGEN AND TARGET DOXYGEN_DOT)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/doxygen.stamp"
        COMMAND DOXYGEN
        ARGS "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
        COMMAND ${CMAKE_COMMAND}
        ARGS -E touch "${CMAKE_CURRENT_BINARY_DIR}/doxygen.stamp"
        DEPENDS
            "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
            include/spacepi/concurrent/AsyncInterrupt.hpp
            include/spacepi/concurrent/ConditionVariable.hpp
            include/spacepi/concurrent/Interrupt.hpp
            include/spacepi/concurrent/RWMutex.hpp
            include/spacepi/concurrent/Semaphore.hpp
            include/spacepi/concurrent/Sleep.hpp
            include/spacepi/concurrent/ThreadPool.hpp
            include/spacepi/concurrent/UniqueConditionVariableLock.hpp
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
            include/spacepi/messaging/Publisher.hpp
            include/spacepi/messaging/RawConnection.hpp
            include/spacepi/messaging/Subscription.hpp
            include/spacepi/package/Module.hpp
            include/spacepi/package/Options.hpp
            include/spacepi/package/PackageConfig.hpp
            include/spacepi/package/User.hpp
            include/spacepi/resource/ADC.hpp
            include/spacepi/resource/BusTransaction.hpp
            include/spacepi/resource/DigitalIO.hpp
            include/spacepi/resource/Filesystem.hpp
            include/spacepi/resource/I2C.hpp
            include/spacepi/resource/Memory.hpp
            include/spacepi/resource/Processor.hpp
            include/spacepi/resource/ResourceFactory.hpp
            include/spacepi/resource/ResourcePtr.hpp
            include/spacepi/resource/SPI.hpp
            include/spacepi/resource/Stream.hpp
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
            include/spacepi/Package.hpp
            include/spacepi/Resource.hpp
            include/spacepi/Util.hpp
            include/SpacePi.hpp
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include"
        COMMENT "Generating SpacePi HTML documentation"
        VERBATIM
    )
    add_custom_target("spacepi-docs"
        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/doxygen.stamp"
    )
    add_dependencies(docs "spacepi-docs")
    if (TARGET PYTHON3 AND TARGET PYTHON3_PIP)
        add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/sphinx.stamp"
            COMMAND PYTHON3
            ARGS -m pip install -q --user -r requirements.txt
            COMMAND ${CMAKE_COMMAND}
            ARGS -E env "BUILD_DIR=${CMAKE_CURRENT_BINARY_DIR}" $<TARGET_FILE:PYTHON3> -m sphinx.cmd.build -M html . "${CMAKE_CURRENT_BINARY_DIR}/sphinx"
            COMMAND ${CMAKE_COMMAND}
            ARGS -E touch "${CMAKE_CURRENT_BINARY_DIR}/sphinx.stamp"
            DEPENDS
                "${CMAKE_CURRENT_BINARY_DIR}/doxygen.stamp"
                docs/conf.py
                docs/index.rst
                docs/requirements.txt
                docs/detailed/architecture.rst
                docs/detailed/core-contrib.rst
                docs/detailed/getting-started.rst
                docs/detailed/networking.rst
                docs/detailed/submodules.rst
                docs/detailed/troubleshooting.rst
                docs/spacepi/concurrent/AsyncInterrupt.rst
                docs/spacepi/concurrent/ConditionVariable.rst
                docs/spacepi/concurrent/Interrupt.rst
                docs/spacepi/concurrent/RWMutex.rst
                docs/spacepi/concurrent/Semaphore.rst
                docs/spacepi/concurrent/Sleep.rst
                docs/spacepi/concurrent/ThreadPool.rst
                docs/spacepi/concurrent/UniqueConditionVariableLock.rst
                docs/spacepi/log/AutoLog.rst
                docs/spacepi/log/ConsoleTarget.rst
                docs/spacepi/log/Entry.rst
                docs/spacepi/log/LogFilterCommand.rst
                docs/spacepi/log/LogFilter.rst
                docs/spacepi/log/Logger.rst
                docs/spacepi/log/LogLevel.rst
                docs/spacepi/log/LogManager.rst
                docs/spacepi/log/LogStream.rst
                docs/spacepi/log/LogTarget.rst
                docs/spacepi/messaging/network/GenericSocketWrapper.rst
                docs/spacepi/messaging/network/MessagingCallback.rst
                docs/spacepi/messaging/network/MessagingSocket.rst
                docs/spacepi/messaging/network/NetworkThread.rst
                docs/spacepi/messaging/network/SocketWrapperCallback.rst
                docs/spacepi/messaging/network/SocketWrapper.rst
                docs/spacepi/messaging/network/SubscriptionID.rst
                docs/spacepi/messaging/Connection.rst
                docs/spacepi/messaging/GenericSubscription.rst
                docs/spacepi/messaging/Network.rst
                docs/spacepi/messaging/Publisher.rst
                docs/spacepi/messaging/RawConnection.rst
                docs/spacepi/messaging/Subscription.rst
                docs/spacepi/package/Module.rst
                docs/spacepi/package/Options.rst
                docs/spacepi/package/PackageConfig.rst
                docs/spacepi/package/User.rst
                docs/spacepi/resource/ADC.rst
                docs/spacepi/resource/Bus.rst
                docs/spacepi/resource/BusTransaction.rst
                docs/spacepi/resource/DigitalIO.rst
                docs/spacepi/resource/Filesystem.rst
                docs/spacepi/resource/I2C.rst
                docs/spacepi/resource/MemoryInfo.rst
                docs/spacepi/resource/Memory.rst
                docs/spacepi/resource/ProcessInfo.rst
                docs/spacepi/resource/Processor.rst
                docs/spacepi/resource/RAMInfo.rst
                docs/spacepi/resource/ResourceFactory.rst
                docs/spacepi/resource/ResourcePtr.rst
                docs/spacepi/resource/SPI.rst
                docs/spacepi/resource/Stream.rst
                docs/spacepi/resource/UART.rst
                docs/spacepi/util/CommandConfigurable.rst
                docs/spacepi/util/Command.rst
                docs/spacepi/util/Exception.rst
                docs/spacepi/util/SharedOrRef.rst
                docs/spacepi/util/TemporalQueue.rst
                docs/spacepi/util/Trie.rst
                docs/spacepi/util/WeakOrRef.rst
                docs/spacepi/Concurrent.rst
                docs/spacepi/Log.rst
                docs/spacepi/Messaging.rst
                docs/spacepi/Package.rst
                docs/spacepi/Resource.rst
                docs/spacepi/Util.rst
                docs/SpacePi.rst
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/docs"
            COMMENT "Generating SpacePi Sphinx documentation"
            VERBATIM
        )
        add_custom_target("spacepi-sphinx"
            DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/sphinx.stamp"
        )
        add_dependencies(docs "spacepi-sphinx")
    endif()
endif()

add_subdirectory(dashboard)
add_subdirectory(examples)
add_subdirectory(router)
