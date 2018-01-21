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

  This example shows how to use Energia BLE
  to connect your project to Blynk.

  NOTE: BLE support is in beta!

 *************************************************************/

//#define BLYNK_DEBUG
#define BLYNK_PRINT Serial
#define BLYNK_RUN_YIELD() { ble.handleEvents(); }

#include <BLE.h>
#include <BlynkSimpleSerialBLE.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup() {
  Serial.begin(115200);

  ble.setLogLevel(BLE_LOG_ERRORS);
  ble.begin();
  ble.serial();
  ble.setAdvertName("Blynk BLE");
  ble.startAdvert();

  Serial.println("Waiting for connections...");

  Blynk.begin(ble, auth);
}

void loop() {
  Blynk.run();
}

