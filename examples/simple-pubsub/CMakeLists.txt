spacepi_message_library(
    example-simplepubsub-messages messages
    SOURCES
        spacepi/example/HelloMessage.proto
        spacepi/example/GoodbyeMessage.proto
    EXAMPLE
)

add_subdirectory(publisher)
add_subdirectory(subscriber)
