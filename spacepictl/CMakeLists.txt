add_executable(spacepictl
    src/FSOperation.cpp
    src/FSOStream.cpp
    src/FSTransaction.cpp
    src/SpacePiCTL.cpp
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
    ${CMAKE_DL_LIBS}
    spacepi
	git2
)

target_compile_definitions(spacepictl PUBLIC BOOST_NO_CXX11_SCOPED_ENUMS=0)

target_include_directories(spacepictl PUBLIC include)

target_precompile_headers(spacepictl PUBLIC
    <SpacePi.hpp>
)

install(TARGETS spacepictl RUNTIME DESTINATION bin)
