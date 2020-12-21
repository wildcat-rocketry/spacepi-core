add_executable(spacepictl
    src/init.cpp
	src/Person.cpp
	src/User.cpp
	src/UserManager.cpp
	src/System.cpp
)

find_package(Boost
    REQUIRED COMPONENTS
    system
    thread
	filesystem
)

target_link_libraries(spacepictl
    Boost::system
    Boost::thread
	Boost::filesystem
)

target_compile_definitions(spacepictl PUBLIC BOOST_NO_CXX11_SCOPED_ENUMS=0)

target_include_directories(spacepictl PUBLIC include)

install(TARGETS spacepictl DESTINATION /usr/bin)
