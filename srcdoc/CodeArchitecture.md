# Code Architecture

### Console

Device support multiple connexion. In that case, the message will be displayed in
all the connexions. The received command will be echoed to all connexions with a prefix
that display the origin of the command. See [Console commands](srcdoc/consoleCommands.md) to
known how to use it.

#### USB Serial

The default connexion to the device is to use the USB cable.
Then use a serial communication at 115200 bauds.

This is equivalent to plug cable to TX RX pins on the board. But, with the USB,
it also provides power to the board.

#### Telnet

If the device network is connected, then the console may also be accessible through a telnet connexion.
To connect to the device simply connect to its IP port 23 with the Telnet protocol.
Only one telnet client at a time is supported by the device.

