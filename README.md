ABOUT
===========

This is an Arduino powered home automation project that allows you to open and close curtains. The motors are controlled by a webpage and can be scheduled to open at various times. This is still a work in progress and much of the functionality is unimplemented. After implementing most of the functionality, I may expand the capabilities to include other home automation features.

I have a hard time getting out of bed in the morning, but I found that if my curtains are open the sunlight makes it easier. Thus, I started work on this project.

HARDWARE
===========

- Arduino Uno
- Ethernet Shield
- Pololu DRV8825 Stepper Motor Driver Carrier
- Bipolar Stepper Motor

A circuit diagram can be found in the arduino directory.

SOFTWARAE
===========

To use the Arduino program, upload it to an Arduion Uno and connect an ethernet cable to the ethernet shield. To test ethernet communication, compile the udp_send_receive utility and send the 'status' command.

COMMANDS
===========

This list will be updated as commands are added:

on - Enables the motor
off - Disables the motor
cw - Runs the motor clockwise for 5 seconds
ccw - Runs the motor counter-clockwise for 5 seconds
status - Returns the current status of the motor

CONTACT
===========

If you want to contact me, you can send me an email at: tcarney@travis-carney.com
