# SpacePi Core Repository

// TODO

## Technical Details

### Networking Protocol

Each message given to the messaging framework is transmitted over the network as a single packet.
They may be transmitted either over a TCP socket or over a UNIX stream socket.
Each packet will always have the following format:

1. A packet length prefix, encoded as a [base 128 varint](https://developers.google.com/protocol-buffers/docs/encoding#varints)
2. The payload of the packet, with a length given by the length prefix

The payload of the packet can be interpreted as a [spacepi::messaging::EncapsulatedMessage](include/spacepi/messaging/EncapsulatedMessage.proto) message.
This `EncapsulatedMessage` contains the following fields:

| Field      | Type   | Description                                          |
|------------|--------|------------------------------------------------------|
| messageID  | uint64 | Used to identify the type of a message               |
| instanceID | uint64 | Used to identify the source/destination of a message |
| payload    | bytes  | The raw data used to parse the message from          |

#### Message IDs

A message ID defines the type of the message being sent.
For example, this may be a set of data that was read from a sensor.
Each protobuf type must have a unique message ID.

Message IDs are specified in a protobuf type as follows:

```protobuf
syntax = "proto3";

import "spacepi/messaging/MessageID.proto";

message MyMessage {
    option (spacepi.messaging.MessageID) = 0x123;
    // ...
}
```

In cases in which some packets may not be able to be transferred due to problems such as bandwidth limitations, messages with a smaller message ID will be transmitted first, as they are considered higher priority.

By convention, these bit fields exist in the message IDs.
However, the SpacePi core code never checks these fields, so they are not required when using the framework.

| Start Bit | End Bit  | Description |
|-----------|----------|-------------|
| LSb       | LSb + 7  | Type ID     |
| LSb + 8   | LSb + 11 | Category ID |
| LSb + 12  | LSb + 13 | Group ID    |

By using this format, there are up to 4 groups IDs, 16 category IDs in each group, and 256 type IDs in each category.
Additionally, since this will result in a 14-bit message ID, it will only consume two bytes when serialized for transmission as a packet.
However, up to 64 bits are available for systems which may need more unique messages than representable using this system.

By convention, the following group IDs exist (smaller group IDs have higher priority):

| Group ID | Usage                               |
|----------|-------------------------------------|
| 0b00     | SpacePi Framework reserved messages |
| 0b01     | Reserved                            |
| 0b10     | Application-specific messages       |
| 0b11     | Messages from example code          |

One example for category IDs may be as follows (again, smaller category IDs have higher priority than larger category IDs within a single group):

| Category ID | Usage                               |
|-------------|-------------------------------------|
| 0b0010      | Error reporting                     |
| 0b0100      | Actions which have physical effects |
| 0b0110      | Telemetry controls                  |
| 0b1000      | Telemetry from sensors              |

For example, if a message has a group ID of `0x3` (messages from example code), a category ID of `0x6` (telemetry controls), and a type ID of `0xAB`, it's message ID would be `0x36AB`.

#### Instance IDs

Instead of identifying the type of message, instance IDs identify the source and/or destination for the message.
For example, if there are multiple of the same kind of sensor connected to the system, different instance IDs would be used for each of the sensors, but their message IDs would all be the same.

Instance IDs are specified in the code where messages are being created, so the instance IDs do not need to be defined in the ProtoBuf files.

If two packets with equal message IDs are received, the packet with a smaller instance ID is considered to have a higher priority.

#### Special Messages

The following messages fall into the "SpacePi Framework reserved messages" group ID discussed above, and are used inside the SpacePi core code for certain tasks.
Do not create any messages which have the same IDs as these, and also do not ever manually send these messages.

##### HelloMessage

The [spacepi::messaging::HelloMessage](include/spacepi/messaging/HelloMessage.proto) is sent when a client connects to a server in order to identify the server software.
This packet can also be used by network filtering software to identify a socket stream as using the SpacePi message protocol.

This message has a messageID of `0x10`.

##### SubscribeRequest

The [spacepi::messaging::SubscribeRequest](include/spacepi/messaging/SubscribeRequest.proto) is sent by the client to the server in order to request to receive messages sent by other clients.

This message has a message ID of `0x20`.
