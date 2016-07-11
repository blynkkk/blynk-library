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
 * This example shows how to use Software Serial
 * to connect your project to Blynk.
 * Be sure to check ordinary Serial example first!!!
 *
 **************************************************************
 *            USB HOWTO: http://tiny.cc/BlynkUSB
 **************************************************************/

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <BlynkSimpleStream.h>

#include <SoftwareSerial.h>
SoftwareSerial SwSerial(2, 3); // RX, TX

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  // Debug console
  Serial.begin(9600);

  // Blynk will work through SoftwareSerial
  SwSerial.begin(9600);
  Blynk.begin(auth, SwSerial);
}

void loop()
{
  Blynk.run();
}

