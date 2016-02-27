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
 * This sketch prints all virtual pin operations!
 *
 **************************************************************/

#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth);
}

// This is called for all virtual pins, that don't have BLYNK_WRITE handler
BLYNK_WRITE_DEFAULT() {
  BLYNK_LOG("V%d input: ", request.pin);
  // Print all parameter values
  for (auto i = param.begin(); i < param.end(); ++i) {
    BLYNK_LOG("* %s", i.asString());
  }
}

// This is called for all virtual pins, that don't have BLYNK_READ handler
BLYNK_READ_DEFAULT() {
  // Generate random response
  int val = random(0, 100);
  BLYNK_LOG("V%d output: %d", request.pin, val);
  Blynk.virtualWrite(request.pin, val);
}

void loop()
{
  Blynk.run();
}

