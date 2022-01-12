# ConsoleCommand

The list of actual console command

## General commands

 | command   | parameter      | description |
 | --------- | :------------: | ------------: |
 | `dmesg`   | n/a            | print kernel messages |
 | `help`    | `<drivername>` | print help on driver, or give the list of drivers |
 | `cfgload` | n/a            | load configuration from files |
 | `cfgsave` | n/a            | save configuration to files |

## File system commands

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

## Network commands

 | command | parameter | description |
 | ------- | :-------: | ----------: |
 |`netinfo`| n/a       | display network information |
 |`netstat`| n/a       | display network status |

## Status led commands

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

## Time commands

If the device is connected to the internet, then it internal time can be synchronized by NTP protocol.

 | command     | parameter | description |
 | ----------- | :-------: | ----------: |
 | `date`      | n/a       | Print the current date |
 | `clockinfo` | n/a       | print time system infos |
 | `clockpool` | `<pool>`  | change the name of the pool server |
 | `clocktz  ` | `<tz>`    | change the time zone (see TZ.h for the format) |

## RunCam commands

It is possible to plug a device that runs with the runcam protocol.
Wire the RX pin of the runcam device to the D5 pin of the Wemos and the TX to D6.

The device is automatically detected.

 | command       | parameter   | description |
 | ------------- | :---------: | ---------- |
 | `runcamDebug` | n/a         | Toggle the debug print in the Multi-Stream. |
 | `runcamInfo`  | n/a         | Print informations about the device. |
 | `runcamCmd`   | `<command>` |   Send a command to the connected device. |
 | `runcamMenu`  | `<key>`     |   Send a 5 key action to the connected device. |

Valid commands are:

 | command  | description |
 | -------- | ----------- |
 | `manual` | Go to manual mode |
 | `ctrl`   | Back to controlled mode |
 | `start`  | Start Camera |
 | `stop`   | Stop Camera |

Valid keys are:

 | key       | description |
 | --------- | ----------- |
 | `open`    | Open the menu |
 | `set`     | Simulate the push on button |
 | `left`    | Simulate the left direction |
 | `right`   | Simulate the right direction |
 | `up`      | Simulate the up direction |
 | `down`    | Simulate the down direction |