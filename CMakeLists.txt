cmake_minimum_required(VERSION 3.0)
project(spacepi-core)
include(cmake/SpacePiSetup.cmake)

include(SpacePiTargets)
spacepi_default_targets(
    CORE
    DOCS
)

include(SpacePiOnce)
spacepi_once(SPACEPI_CORE)

spacepi_module(
    CORE LIBRARY NAME spacepi
    SOURCES
        src/log/LogLevel.cpp
    INCLUDE include
    PRECOMPILE <spacepi/log/LogLevel.hpp>
    LINK spacepi-platform
)

add_subdirectory(dashboard)
add_subdirectory(examples)
#add_subdirectory(journaler)
add_subdirectory(protoc)
#add_subdirectory(router)

find_package(Doxygen)
if (TARGET Doxygen::doxygen AND TARGET Doxygen::dot)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/doxygen.stamp"
        COMMAND Doxygen::doxygen
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
            include/spacepi/log/FileTarget.hpp
            include/spacepi/log/LogCommand.hpp
            include/spacepi/log/LogFilter.hpp
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
    find_package(Python3 COMPONENTS Interpreter)
    if (TARGET Python3::Interpreter)
        get_property(loc TARGET Python3::Interpreter PROPERTY IMPORTED_LOCATION)
        execute_process(
            COMMAND "${loc}" -m pip help
            RESULT_VARIABLE found
            OUTPUT_QUIET
            ERROR_QUIET
        )
        if (found EQUAL 0)
            add_custom_command(
                OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/sphinx.stamp"
                COMMAND Python3::Interpreter
                ARGS -m pip install -q --user -r requirements.txt
                COMMAND ${CMAKE_COMMAND}
                ARGS -E env "BUILD_DIR=${CMAKE_CURRENT_BINARY_DIR}" $<TARGET_FILE:Python3::Interpreter> -m sphinx.cmd.build -M html . "${CMAKE_CURRENT_BINARY_DIR}/sphinx"
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
                    docs/spacepi/log/FileTarget.rst
                    docs/spacepi/log/LogCommand.rst
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
endif()
