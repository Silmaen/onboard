# Remote Control

The remote control consist of decoding the input on the 4 connected wire.

The first wire is use to control the recording of the camera: HIGH means the camera should be recording,
LOW for Stop recording.

Instruction are decoded when the input change of state.

## input values

 | input as binary | meaning   |
 | --------------- | --------- |
 |  `0000`         | idle      |
 |  `1xxx`         | recording |
 |  `0100`         | left      |
 |  `0101`         | right     |
 |  `0110`         | up        |
 |  `0111`         | down      |
 |  `0011`         | set       |