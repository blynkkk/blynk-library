/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Social networks:            http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  This example shows how to use Arduino + RedBearLab BLE Mini
  to connect your project to Blynk.

  NOTE: BLE support is in beta!

 *************************************************************/

#define BLYNK_USE_DIRECT_CONNECT

#define BLYNK_PRINT Serial

#include <BlynkSimpleSerialBLE.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

#define SerialBLE  Serial1      // Set Serial object

void setup()
{
  // This is for debug prints
  Serial.begin(9600);

  Serial.println("Waiting for connections...");

  SerialBLE.begin(57600);       // BLE Mini uses baud 57600
  Blynk.begin(SerialBLE, auth);
}

void loop()
{
  Blynk.run();
}

