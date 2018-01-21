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

  This example shows how to use Simblee BLE
  to connect your project to Blynk.

  NOTE: BLE support is in beta!

 *************************************************************/

//#define BLYNK_USE_DIRECT_CONNECT

#define BLYNK_PRINT Serial

#include <BlynkSimpleSimbleeBLE.h>
#include <SimbleeBLE.h>

char auth[] = "YourAuthToken";

void setup()
{
  Serial.begin(9600);

  SimbleeBLE.deviceName = "Simblee";
  SimbleeBLE.advertisementInterval = MILLISECONDS(300);
  SimbleeBLE.txPowerLevel = -20;  // (-20dbM to +4 dBm)

  // start the BLE stack
  SimbleeBLE.begin();

  Serial.println("Waiting for connections...");

  Blynk.begin(auth);
}


void loop()
{
  Blynk.run();
}

