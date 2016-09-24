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
 *
 * This example shows how to use Arduino 101 CurieBLE
 * to connect your project to Blynk.
 *
 * Note: This requires CurieBLE library from http://librarymanager
 *
 * NOTE: BLE support is in beta!
 *
 **************************************************************/

//#define BLYNK_DEBUG
#define BLYNK_PRINT Serial

//#define BLYNK_USE_DIRECT_CONNECT

#include <BlynkSimpleCurieBLE.h>
#include <CurieBLE.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

BLEPeripheral  blePeripheral;

void setup() {
  Serial.begin(9600);
  delay(1000);

  blePeripheral.setLocalName("Blynk");
  blePeripheral.setDeviceName("Blynk");
  blePeripheral.setAppearance(384);

  Blynk.begin(auth, blePeripheral);

  blePeripheral.begin();

  Serial.println("Waiting for connections...");
}

void loop() {
  Blynk.run();
  blePeripheral.poll();
}

