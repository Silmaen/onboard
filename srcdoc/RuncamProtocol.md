# RunCam Protocols

## Official Protocol

The RunCam protocol can be found [here](https://support.runcam.com/hc/en-us/articles/360014537794-RunCam-Device-Protocol).

## RunCam with Caddx Loris

### Testing the communication

To summarize what is working with Caddx Loris:

The acknowledgement message is `0xCCA5` so, simply the header and the crc code.
We named it `ACK`

| command       | response   | description |
| ------------- | ---------- | ---------- |
| `0x00`        | `0x010900` | Get Device informations |
| `0x0100`  | n/a | in camera mode: no effet // in menu: validate |
| `0x0101`  | n/a | simulate power button push (in camera mode: start/stop recording // in menu: no effect) |
| `0x0102`  | n/a | in camera mode: open menu // in menu: move up |
| `0x0201`  | `ACK` | in camera mode: open menu // in menu: validate |
| `0x0202`  | `ACK` | in camera mode: no effet // in menu: left |
| `0x0203`  | `ACK` | in camera mode: no effet // in menu: right |
| `0x0204`  | `ACK` | in camera mode: no effet // in menu: up |
| `0x0205`  | `ACK` | in camera mode: no effet // in menu: down |
| `0x03`  | `ACK` | in camera mode: no effet // in menu: no effect |
| `0x0401`  | `11` | no effet |
| `0x0401`  | `21` | no effet |

No other command have effect.

Note that while the camera is recording, it is impossible to go to menu.

### Comments

The table shows that few commands are available. In particular,
it is impossible to access to the camera clock (would have been useful for SD
card files management). We do not have access to any settings or status,
we can only deduce them.

The response message to device information `0x010900` means that the device protocol is 1;
the next 2 byte are the component of the 16bit word and are sent in LSB first order. So the
real number in hexadecimal is `0x0009` in binary: `0b000000001001`. According to the RunCam
protocol this means that the only available features are `SIMULATE_POWER_BUTTON` and
`SIMULATE_5_KEY_OSD_CABLE`, this corresponds to what has been observed.

In the camera setting it is then important to set up the 'Auto recording' field to false.
So the driver can assume that at boot time the recording is off.

More information about the driver can be found [here](RuncamDriver.md).
