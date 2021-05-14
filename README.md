# Project onboard

Driver for my drone's onboard device.

An ESP8266 device plugged to TX/RX of the Caddx Loris Camera acts as a communication relay between Camera, 
flight controler and ground station.

## Device connection

The default connexion to the device is to use the USB cable.
Then use a serial communication at 115200 bauds.

## Console command

The list of actual console command:

 * General commands :

 | command | parameter  | description |
 | ------- | :--------: | ------------: |
 | `dmesg` | n/a        | print kernel messages |

 * file system commands

 | command | parameter | description |
 | ------- | :-------: | ------------: |
 | `pwd`   | n/a       | print curent directory |
 | `ls`    | n/a       | list the content of the current directory |
 | `cd`    | `<path>`  | change the current directory | 
 | `mkdir` | `<path>`  | create a new directory |
 | `rm`    | `<path>`  | remove a file or a directory (with its content) |

The `<path>` can be absolute as `/foo/bar` or relatives to the current directory.
It is possible to use `.` for current path and `..` for parent path.
(example: if current dir is `foo`, then a path like `../bar/./bbb` is equivalent 
to `/bar/bbb`). Please note that the paths are limited to 31 characters. 

 * network commands

 | command | parameter | description |
 | ------- | :-------: | ----------: |
 |`netinfo`| n/a       | display network informations |