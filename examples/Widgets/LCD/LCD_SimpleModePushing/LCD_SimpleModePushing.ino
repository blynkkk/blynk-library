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
 * Output any data on LCD widget!
 *
 * App project setup:
 *
 *   LCD widget, SIMPLE mode, in widget settings :
 *
 *   - Select pin V0 for zero pin
 *   - Select pin V1 for first pin
 *   - Change "Reading Frequency" to PUSH mode
 *   - Type into first edit field "/pin0/ seconds"
 *   - Type into second edit field "/pin1/ millis"
 *
 **************************************************************/
#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <SimpleTimer.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

SimpleTimer timer;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth);

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSeconds);
  // Setup a function to be called every second
  timer.setInterval(1000L, sendMillis);


  while (Blynk.connect() == false) {
    // Wait until connected
  }
}

void sendSeconds() {
  Blynk.virtualWrite(V0, millis() / 1000);
}

void sendMillis() {
  Blynk.virtualWrite(V1, millis());
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates SimpleTimer
}

