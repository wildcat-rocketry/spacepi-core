cmake_minimum_required(VERSION 2.15)

add_executable(spacepi-init
    src/init.cpp
	src/Person.cpp
	src/User.cpp
	src/UserManager.cpp
	src/System.cpp
)

find_package(Boost
    REQUIRED COMPONENTS
    # asio
    # beast
    system
    thread
	filesystem
)

target_link_libraries(spacepi-init
    Boost::system
    Boost::thread
	Boost::filesystem
)


target_compile_definitions(spacepi-init PUBLIC BOOST_NO_CXX11_SCOPED_ENUMS=0)

target_include_directories(spacepi-init PUBLIC include)
