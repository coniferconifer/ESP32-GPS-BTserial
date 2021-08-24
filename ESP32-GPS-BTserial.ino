/*
 ESP32 GPS-BT-server with watch dog timer

 Hookup GPS module to ESP32 and read out NMEA by Android phone via Bluetooth 
 Serial Port Profile.
 NMEA can be monitored by ESP32-USB serial port and BT serial terminal on Android
 
 When Serial.BT stops , watch dog timer reboot this program.
 This program requires Android phone has the Bluetooth Serial Port Profile 
 to connect to ESP32-GPS.
 Application : run Lefebure to replace from Android phone's embeded GPS to your GPS
 
  https://play.google.com/store/apps/details?id=com.lefebure.ntripclient&hl=ja&gl=US

 GPS TX should be connected to GPIO_NUM_16 of ESP32
 GPS used to test : M5stack GPS module (using AT6558) , connected by 9600bps serial

 Ref. http://arduiniana.org/libraries/tinygpsplus/
      https://github.com/espressif/arduino-esp32/blob/master/libraries/BluetoothSerial/
      
 Copyright 2021(C) coniferconifer

 MIT License
 
*/
#include <Ticker.h>      // timer 

#include <TinyGPS++.h>
#define VERSION "20210823"
TinyGPSPlus gps;
#define GPS_BAUD 9600
HardwareSerial ss(2); // GPIO_NUM_16 for RX ( wired to GPS TX), GPIO_NUM_17 for TX (not used now)
// https://github.com/espressif/arduino-esp32/blob/master/libraries/BluetoothSerial/examples/SerialToSerialBTM/SerialToSerialBTM.ino
#include "BluetoothSerial.h"
BluetoothSerial SerialBT; 
TaskHandle_t Task1;

//Ticker  ticker;
//bool bReadyTicker = false;
const int iIntervalTime = 3;  // watch dog interval in sec
long lastGPSread=0;

//  Watch dog for BT serial
void codeForBTserial(void * parameter)
{
  portTickType xLastWakeTime;
  const portTickType xFrequency = 1000;//run here every 1000msec
  while (1) {
    xLastWakeTime = xTaskGetTickCount();// Initialise the xLastWakeTime variable with the current time.
    if ( (millis() - lastGPSread) > iIntervalTime * 1000  ) {
      Serial.println();
      Serial.println();
      Serial.printf("Serial BT hang? rebooting now %d \r\n", lastGPSread);
      ESP.restart();
    }
    vTaskDelayUntil( &xLastWakeTime, xFrequency );
  }
}
/*void kickRoutine() {
  bReadyTicker = true;
  //Serial.println("******kickRoutine()**************");
  if ( (millis()-lastGPSread) > iIntervalTime*1000*2 ) {
    Serial.println();
    Serial.println();
    Serial.printf("Serial BT hang? rebooting now %d \r\n",lastGPSread);
    ESP.restart();
  }

}
*/
void setup() {
  Serial.begin(115200);
  while (!Serial) continue;
  Serial.print("ESP32-GPS-BTserial.ino ");Serial.println(VERSION);
  Serial.println("setup Android terminal to pair with GPS-BT-server");
  SerialBT.begin("GPS-BT-server");
  Serial.println("The device started, now you can pair it with bluetooth!");
  ss.begin(GPS_BAUD);
 // start timer interrupt
 // ticker.attach(iIntervalTime, kickRoutine);
  lastGPSread=millis();
  xTaskCreatePinnedToCore( codeForBTserial, "BTserialWatch", 4000, NULL, 1, &Task1, 1); //core 1
}

void loop() {
  char c;
  
  if (ss.available() > 0) {
    c = ss.read(); //
    Serial.write(c); // monitor for GPS application , ex. ublox u-center
    lastGPSread=millis();
    SerialBT.write(c); // NMEA can be monitored by BT-Serial application for Android
    if (gps.encode(c)) {
      // getGPSInfo();
    }
  }
}
