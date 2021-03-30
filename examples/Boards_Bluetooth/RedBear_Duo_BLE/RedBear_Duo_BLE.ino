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

  This example shows how to use RedBear Duo BLE
  to connect your project to Blynk.

  NOTE: BLE support is in beta!

 *************************************************************/

#if defined(ARDUINO)
  SYSTEM_MODE(MANUAL);    // If Arduino - do not connect to Particle cloud
#else
  SYSTEM_MODE(AUTOMATIC); // Otherwise, connect to Particle cloud
#endif

#define BLYNK_USE_DIRECT_CONNECT

#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
//#define BLYNK_TEMPLATE_ID   "YourTemplateID"


#include <BlynkSimpleRedBear_Duo_BLE.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup() {
  Serial.begin(9600);
  delay(5000);

  Serial.println("Waiting for connections...");

  Blynk.begin(auth);
}

void loop() {
  Blynk.run();
}

// Next functions are optional.
// Add ZeRGBa Widget on V0 to control onboard LED.
BLYNK_CONNECTED() {
  RGB.control(true);
}

BLYNK_WRITE(V0) {
  int r = param[0].asInt();
  int g = param[1].asInt();
  int b = param[2].asInt();
  RGB.color(r, g, b);
}

