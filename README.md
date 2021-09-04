# ESP32 GPS-BT-server with watch dog timer

Hookup GPS module to ESP32 and read out NMEA by Android phone via Bluetooth 
Serial Port Profile.
NMEA can be monitored by ESP32-USB serial port and BT serial terminal on Android.
 
When Serial.BT stops unexpectedly, the watch dog timer reboots ESP32 board.

This program requires Android phone has the Bluetooth Serial Port Profile to connect to ESP32-GPS.


## Application 

![system](https://github.com/coniferconifer/ESP32-GPS-BTserial/blob/main/block.drawio.png)

Use your external GPS module as Android's internal GPS.

Step1. Configure mock-location on Android.

Ref. https://eos-gnss.com/knowledge-base/configuring-mock-location-on-android-with-your-arrow-gnss-receiver

Step2. Run Lefebure NTRIP Client on Android to replace from Android phone's embeded GPS to your GPS.
No need to set up NTRIP server if your GPS module does not support RTK GPS.
 
Ref. https://play.google.com/store/apps/details?id=com.lefebure.ntripclient&hl=ja&gl=US


## How to wire your GPS module
https://shop.m5stack.com/products/mini-gps-bds-unit

GPS TX should be connected to GPIO_NUM_16 of ESP32

GPS used to test : M5stack GPS module (using AT6558) , connected by 9600bps serial.


Tested with https://shop.m5stack.com/products/mini-gps-bds-unit and Android 11 phone.

Android BT serial terminal: https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal&hl=ja&gl=US

Monitoring tool : "NMEA Monitor for windows" http://4river.a.la9.jp/gps/index.htm

## Libraries 
TinyGPS+

Ref. http://arduiniana.org/libraries/tinygpsplus/

Bluetooth Serial Port Profile at ESP32

Ref. https://github.com/espressif/arduino-esp32/blob/master/libraries/BluetoothSerial/
      
## License

Copyright 2021(C) coniferconifer

MIT License

## dual GPS BT serial 
https://github.com/coniferconifer/ESP32-dual-GPS-BTserial