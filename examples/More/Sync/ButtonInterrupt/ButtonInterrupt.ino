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
 * This example shows how to monitor a button state
 * using interrupts mechanism.
 *
 * App project setup:
 *   LED widget on V1
 *
 **************************************************************/

#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

WidgetLED led1(V1);

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth);

  // Make pin 2 HIGH by default
  pinMode(2, INPUT_PULLUP);
  // Attach INT to our handler
  attachInterrupt(digitalPinToInterrupt(2), checkPin, CHANGE);
}

void checkPin()
{
  // Invert state, since button is "Active LOW"
  if (digitalRead(2)) {
    led1.off();
  } else {
    led1.on();
  }
}

void loop()
{
  Blynk.run();
}

