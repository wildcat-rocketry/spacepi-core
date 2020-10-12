spacepi_message_library(example-simplepubsub-messages ${CMAKE_CURRENT_SOURCE_DIR}/messages
    spacepi/example/HelloMessage.proto
    spacepi/example/GoodbyeMessage.proto
)

add_subdirectory(publisher)
add_subdirectory(subscriber)
