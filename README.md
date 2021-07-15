# Project onboard

Driver for my drone's onboard device.

An ESP8266 device plugged to TX/RX of the Caddx Loris Camera acts as a communication relay between Camera, 
flight controller and ground station.

## Needed tools

Build this project you will need [PlatformIO](https://docs.platformio.org/en/latest/).

We ar using the [ESP8266 Arduino Framework](https://arduino-esp8266.readthedocs.io/en/latest/)

## Device connection

Device support multiple connexion. In that case, the massage will be displayed in
all the connexions. The received command will be echoed to all connexions with a prefix
that display the origin of the command.

### USB Serial

The default connexion to the device is to use the USB cable.
Then use a serial communication at 115200 bauds.

This is equivalent to plug cable to TX RX pins on the board. but, with the USB, 
it also provides power to the board.

### Telnet

If the device network is connected, then the console may also be accessible through a telnet connexion.
To connect to the device simply connect to its IP port 23 with the Telnet protocol.
Only one telnet client at a time is supported by the device.

### Wiring

Here is the basic wiring of the Wemos D1 with the other peripherals:
![](doc/wiring.png)

## Console command

The list of actual console command:

### General commands :

 | command   | parameter      | description |
 | --------- | :------------: | ------------: |
 | `dmesg`   | n/a            | print kernel messages |
 | `help`    | `<drivername>` | print help on driver, or give the list of drivers |
 | `cfgload` | n/a            | load configuration from files |
 | `cfgsave` | n/a            | save configuration to files |

### File system commands

 | command | parameter | description |
 | ------- | :-------: | ------------: |
 | `pwd`   | n/a       | print current directory |
 | `ls`    | n/a       | list the content of the current directory |
 | `cd`    | `<path>`  | change the current directory | 
 | `mkdir` | `<path>`  | create a new directory |
 | `rm`    | `<path>`  | remove a file or a directory (with its content) |
 | `cat`   | `<path>`  | print the content of a file |

The `<path>` can be absolute as `/foo/bar` or relatives to the current directory.
It is possible to use `.` for current path and `..` for parent path.
(example: if current dir is `foo`, then a path like `../bar/./bbb` is equivalent 
to `/bar/bbb`). Please note that the paths are limited to 31 characters. 

### Network commands

 | command | parameter | description |
 | ------- | :-------: | ----------: |
 |`netinfo`| n/a       | display network information |
 |`netstat`| n/a       | display network status |

### Status led commands
   
 | command | parameter | description |
 | ------- | :-------: | ----------: |
 |`led`    | n/a       | print the current LED state |
 |`led`    | `<state>` | change the LED to given state |

valid sates are:

 | state name   |  description |
 | ------------ |  ----------: |
 |`off`         |  LED off     |
 |`solid`       |  LED on |
 |`blink`       |  LED is slowly blinking |
 |`fastblink`   |  LED is blinking twice faster |
 |`twopulse`    |  LED do 2 pulses then wait |
 |`threepulse`  |  LED do 3 pulses then wait |
 |`fasterblink` |  LED is continuously pulsing |

### Time commands

If the device is connected to the internet, then it internal time can be synchronized by NTP protocol.

 | command     | parameter | description |
 | ----------- | :-------: | ----------: |
 | `date`      | n/a       | Print the current date |
 | `clockinfo` | n/a       | print time system infos |
 | `clockpool` | `<pool>`  | change the name of the pool server |
 | `clocktz  ` | `<tz>`    | change the time zone (see TZ.h for the format) |

### RunCam commands

It is possible to plug a device that runs with the runcam protocol.
Wire the RX pin of the runcam device to the D5 pin of the Wemos and the TX to D6.

#### RunCam Protocols

The RunCam protocol can be found [here](https://support.runcam.com/hc/en-us/articles/360014537794-RunCam-Device-Protocol). 

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

The table shows that few commands are available. In particular, 
it is impossible to access to the camera clock (would have been useful for SD 
card files management). We do not have access to any settings or status, 
we can only deduce them.

The response message to device information `0x010900` means that the device protocol is 1;
the next 2 byte are the component of the 16bit word and are sent in LSB first order. So the 
real number in hexadecimal is `0x0009` in binary: `0b000000001001`. According to the RunCam
protocol this means that the only available features are `SIMULATE_POWER_BUTTON` and 
`SIMULATE_5_KEY_OSD_CABLE`, this corresponds to what has been observed. 

In the camera setting it is then important to setup the 'Auto recording' field to false.
So the driver can assume that at boot time the recording is off.

#### Device Statuses and functions

We set up different statuses for the camera device:

 | status         |  description |
 | -------------- |  :---------- |
 | `DISCONNECTED` |  The device cannot be contacted. |
 | `READY`        |  Camera mode. |
 | `RECORDING`    |  Camera mode and recording in progress. |
 | `MENU`         |  The camera is in menu. |
 | `MANUAL`       |  The camera is manually controlled. |

As there is no access to any camera information, we need to simulate the navigation 
through menus. This allows to detect the closing of the menu.

Here are a list of utility functions:

 | status           | Required State | new state   |  description |
 | ---------------- | -------------- | ----------- | :---------- |
 | `getDeviceInfo`  | all            | if (timeout) { `DISCONNECTED` } elif ( `DISCONNECTED` & response) { `READY`} else {no change} | send an info request,wait for response. (can be used to check connection) |
 | `setManual`      | `READY`, `RECORDING` & `MENU` | `MANUAL` | Set the camera to manual mode do not track for any camera change |
 | `unsetManual`    | `MANUAL`       | `READY`     | Restore Camera in `READY` after manual operations |
 | `resetState `    | all            | `READY`     | Reset the state of the camera to `READY` state |
 | `startRecording` | `READY`        | `RECORDING` | Start the video recording. |
 | `stopRecording`  | `RECORDING`    | `READY`     | Stop the video recording. |
 | `openMenu`       | `READY`        | `MENU`      | Open the camera menu, begin the track of the menu navigation. |
 | `moveLeft`       | `MENU`         | no change or `READY` | Navigate left in the menu (similar to validate). |
 | `moveRight`      | `MENU`         | no change or `READY` | Navigate right in the menu (similar to validate). |
 | `moveset`        | `MENU`         | no change or `READY` | Navigate set in the menu (similar to validate). |
 | `moveUp`         | `MENU`         | no change   | Navigate up in the menu. |
 | `moveDown`       | `MENU`         | no change   | Navigate down in the menu. |


#### Device control

The device is automatically detected.

 | command       | parameter   | description |
 | ------------- | :---------: | ---------- |
 | `runcamDebug` | n/a         | Toggle the debug print in the Multi-Stream. |
 | `runcamInfo`  | n/a         | Print informations about the device. |
 | `runcamCmd`   | `<command>` |   Send a command to the connected device. |
 | `runcam5key`  | `<key>`     |   Send a 5 key action to the connected device. |

Valid commands are:

 | command | description |
 | ------- | ----------- |
 | `wifi`  | Simulate the wifi button |
 | `power` | Simulate the power button |
 | `mode`  | Change camera mode |
 | `start` | Start Camera |
 | `stop`  | Stop Camera |

Valid keys are:

 | key       | description |
 | --------- | ----------- |
 | `open`    | Begin connexion to the 5key pad |
 | `close`   | Close connexion to the 5key pad |
 | `set`     | Simulate the push on button |
 | `left`    | Simulate the left direction |
 | `right`   | Simulate the right direction |
 | `up`      | Simulate the up direction |
 | `down`    | Simulate the down direction |
 | `release` | Simulate the down direction |
