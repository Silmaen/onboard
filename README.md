# Project onboard

Driver for my drone's onboard device.

An ESP8266 device plugged to TX/RX of the Caddx Loris Camera acts as a communication relay between Camera, 
flight controller and ground station.

## Device connection

Device support multiple connexion. In that case, the massage will be displayed in
all the connexions. The received command will be echoed to all connexions with a prefix
that display the origin of the command.

### USB Serial

The default connexion to the device is to use the USB cable.
Then use a serial communication at 115200 bauds.

### Telnet

If the device network is connected, then the console may also be accessible through a telnet connexion.
To connect to the device simply connect to its IP port 23 with the Telnet protocol.
Only one telnet client at a time is supported by the device.


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

 | command | parameter | description |
 | ------- | :-------: | ----------: |
 |`date`   | n/a       | Print the current date |

