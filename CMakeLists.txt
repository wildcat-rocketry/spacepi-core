add_executable(setup-deploy-key
    src/main.cpp
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

target_link_libraries(setup-deploy-key
    Boost::context
    Boost::exception
    Boost::fiber
    Boost::program_options
    Boost::stacktrace_backtrace
    Boost::system
    ${Protobuf_LIBRARIES}
    core-messages
)
target_include_directories(setup-deploy-key PUBLIC include)
