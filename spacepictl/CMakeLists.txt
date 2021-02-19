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
    spacepi
	git2
)

target_include_directories(spacepictl PUBLIC include)

target_precompile_headers(spacepictl REUSE_FROM spacepi_pch)

install(TARGETS spacepictl RUNTIME DESTINATION bin)
