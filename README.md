# Onkyo / Integra eISCP Bridge

Control your old AVR via WiFi. No need for a newer one!
## Purpose
This can be used if you want to control an Onkyo / Integra AVR via Ethernet 
(for example through python / Home Assistant) but
 - It does only have an RS-232 Port
 - You want to use  WiFi instead of Ethernet

With (very inexpensive) hardware and this piece of code you can convert the serial port 
communication (ISCP) to Onkyos eISCP protocol.
This will happen completely transparent for the client, there should literally be no difference
between using this and the native eISCP implementation.

## Usecases (examples)

- Turn on other A/V equipment when the AVR turns on
- Control AVR volume
- Turn on Zone2 Equipment only if Zone2 is active
- Get and Set the active Input Channel
- Get and Set the active Sound Profile
- ... And much more.

## Required Hardware

- ESP8266 WeMosD1 Mini (other 8266 Boards should also work)
- UART Level Shifter (TTL 3.3V -> RS-232), for example MAX3232 Breakout
- Serial Cable

The Tx/Rx of the MAX232 chip have to be connected to the D1/D2 Pins of the WeMos Board. (Which Correlate to GPIO4/5, confusingly.)

## Features

- eISCP and ISCP stream-to-packet layer (look at the C files, they might be useful for other projects)
- transparent conversion between the two formats
- compatiblity with Home Assistant and onkyo-eiscp python module (however all eISCP Cients should work)
- Support for Auto-Discovery

## Planned Features

- currently, auto-Discovery always shows the Receiver as TX-SR705 (Can be changed in udp_server.c). It should be easily possible to dynamically read the Receiver Type on Boot-Up
- Direct MQTT control. This would completely circumvent eISCP, and might be the best solution for new control systems.
- Maybe a custom PCB (that would just plug into any Onkyo AVR. This would include a fan 
controller, because AVRs tend to run hot, and cooling them prolongs the life greatly.)

## License
- GPLv3
- For commercial use (including different licensing) contact me.