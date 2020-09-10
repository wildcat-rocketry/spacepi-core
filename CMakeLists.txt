cmake_minimum_required(VERSION 3.0)
project(spacepi-core)
include(cmake/CoreRepo.cmake)

spacepi_message_library(core-messages ${SPACEPI_CORE_SOURCE_DIR}/include
    spacepi/subscribe.proto
    spacepi/message-id.proto
)

#add_library(spacepi SHARED
#)
#target_include_directories(spacepi PUBLIC ${SPACEPI_CORE_SOURCE_DIR}/include)
#target_link_libraries(spacepi core-messages)
