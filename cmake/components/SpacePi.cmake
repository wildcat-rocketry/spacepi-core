cmake_minimum_required(VERSION 3.11)

file(WRITE "${CMAKE_BINARY_DIR}/.gitignore" "*")

include("${CMAKE_CURRENT_LIST_DIR}/Version.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Once.cmake")

spacepi_once(CORE_CMAKE)

get_filename_component(parent "${CMAKE_CURRENT_LIST_DIR}" DIRECTORY)
get_filename_component(SPACEPI_CORE_SOURCE_DIR "${parent}" DIRECTORY CACHE)

add_custom_target(docs)
add_custom_target(full DEPENDS docs)

include("${CMAKE_CURRENT_LIST_DIR}/Compat.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Options.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/AuxTarget.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/AuxLibrary.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/AuxProgram.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Dependency.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Dependencies.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Directory.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Extension.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/MessageLibrary.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Module.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Package.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Target.cmake")
