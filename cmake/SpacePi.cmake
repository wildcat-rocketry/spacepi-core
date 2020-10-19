file(WRITE "${CMAKE_BINARY_DIR}/.gitignore" "*")

include(${CMAKE_CURRENT_LIST_DIR}/Version.cmake)
if (NOT SPACEPI_CORE_VERSION)
    set(SPACEPI_CORE_VERSION "${version}" CACHE INTERNAL "")
elseif (NOT "${SPACEPI_CORE_VERSION}" STREQUAL "${version}")
    message(FATAL_ERROR
        "SpacePi Core version mismatch!\n"
        "Loaded build script version: ${SPACEPI_CORE_VERSION} (from ${SPACEPI_CORE_SOURCE_DIR})\n"
        "Conflicting build script version: ${version} (from ${CMAKE_CURRENT_LIST_DIR}/../)"
    )
endif()

if (NOT SPACEPI_CORE_SOURCE_DIR)
    set(SPACEPI_CORE_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../" CACHE INTERNAL "")
endif()

if ("${SPACEPI_CORE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_LIST_DIR}/../")
    include("${SPACEPI_CORE_SOURCE_DIR}/cmake/Common.cmake")
endif()
