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
 * This example shows how to use BBC Micro:Bit
 * to connect your project to Blynk.
 *
 * Note: This requires installing:
 *   https://github.com/sandeepmistry/arduino-nRF5
 *   and
 *   https://github.com/sandeepmistry/arduino-BLEPeripheral
 *
 * 1. Select: Tools -> SoftDevice -> S110
 * 2. Select: Tools -> Programmer -> CMSIS-DAP
 * 3. Select: Tools -> nRF5 Flash SoftDevice
 * 4. Read and Accept License
 * 5. Verify and Upload Sketch
 *
 * NOTE: BLE support is in beta!
 *
 **************************************************************/

//#define BLYNK_DEBUG
#define BLYNK_PRINT Serial

#define BLYNK_USE_DIRECT_CONNECT

#include <BlynkSimpleSerialBLE.h>
#include <SPI.h>
#include <BLEPeripheral.h>
#include "BLESerial.h"

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Create ble serial instance, parameters are ignored for MicroBit
BLESerial SerialBLE(0, 0, 0);

void setup() {
  Serial.begin(9600);

  SerialBLE.setLocalName("Blynk");
  SerialBLE.setDeviceName("Blynk");
  SerialBLE.setAppearance(0x0080);
  SerialBLE.begin();

  Blynk.begin(auth, SerialBLE);

  Serial.println("Waiting for connections...");
}

void loop() {
  SerialBLE.poll();

  if (SerialBLE) {    // If BLE is connected...
    Blynk.run();
  }
}

