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

  This example shows how to use Arduino Nano 33 BLE
  to connect your project to Blynk.

  Note: This requires ArduinoBLE library
    from http://librarymanager/all#ArduinoBLE
    or https://github.com/arduino-libraries/ArduinoBLE

  NOTE: BLE support is in beta!

 *************************************************************/

#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
//#define BLYNK_TEMPLATE_ID   "YourTemplateID"


#include <BlynkSimpleArduinoNanoBLE.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup() {
  Serial.begin(9600);

  BLE.setAppearance(0x0080);
  Blynk.begin(auth, "Blynk");

  Serial.println("Waiting for connections...");
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  if (central) {    // If BLE is connected...
    Blynk.run();
    BLE.poll();
  }
}