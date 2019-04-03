This is our current data acquisition software file respository, written for Arduino.
A couple notes on what it does and what libraries are needed:
In short, the code collects data from inertial measurement units, linear displacement
sensors, and RPM sensors. It takes that data and writes onto an SD card.
This code is designed to work on a Teensy 3.6. To use the AcquireData code,
you first need to install the Teensyduino add-on and libraries.
The link can be found by searching Google for Teensyduino.

After installing Teensyduino and the relevant libraries, the Sparkfun LSM6DS3 IMU
library from this respository needs to be installed from this library too. 
Note for this library, it is configured using a Wire1 libraries on the Teensyduino,
and is configured for IMUs on the SCL1 and SDA1 ports, not the SCL0 and SDA0 ports. 
For more information on the code itself, see the comments and GTOR documentation. 