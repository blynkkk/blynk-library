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
 * This example shows how to use ordinary Arduino Serial
 * to connect your project to Blynk.
 * Feel free to apply it to any other example. It's simple!
 *
 * Note: This requires STM32duino core:
 *   https://github.com/rogerclarkmelbourne/Arduino_STM32/wiki/Installation
 *
 * Note: You may need to wait up to 30 seconds after board starts,
 *       to allow USB serial to initialize completely.
 *
 **************************************************************
 *            USB HOWTO: http://tiny.cc/BlynkUSB
 **************************************************************/

#define BLYNK_PRINT Serial1
#include <BlynkSimpleStream.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  // Debug console
  Serial1.begin(9600);

  // Blynk will work through Serial
  Serial.begin(9600);
  Blynk.begin(auth, Serial);
}

void loop()
{
  Blynk.run();
}

