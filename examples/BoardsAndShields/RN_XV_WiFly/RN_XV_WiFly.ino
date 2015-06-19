/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social networks:            http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************
 * This example shows how to use WiFly RN-XV
 * to connect your project to Blynk.
 *
 * For this example you need WiFly_Shield library:
 *   https://github.com/sparkfun/WiFly-Shield/tree/master/Libraries/Arduino
 *
 * You may need to adjust the line in the WiFlyDevice.cpp:
 *   if findInResponse("\r\nwifly-GSX Ver", 1000)) {
 * Please replace the string with the model of your shield.
 *
 * Note: Ensure a stable serial connection!
 *       Hardware serial is preferred.
 *       Firmware version 4.41 or later is preferred.
 *
 * Change WiFi ssid, pass, and Blynk auth token to run :)
 * Feel free to apply it to any other example. It's simple!
 *
 **************************************************************/

//#define BLYNK_DEBUG
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <WiFly.h>
#include <BlynkSimpleWiFly.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

#define WiFlySerial Serial1
// This can be a SoftwareSerial object:
//#include <SoftwareSerial.h>
//SoftwareSerial WiFlySerial(2, 3); // RX, TX

void setup()
{
  Serial.begin(9600);       // Set console baud rate
  delay(10);
  WiFlySerial.begin(9600);  // Set your RN-XV baud rate
  delay(10);

  WiFly.setUart(&WiFlySerial);

  Blynk.begin(auth, "ssid", "pass");
}

void loop()
{
  Blynk.run();
}

