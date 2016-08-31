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
 * This example shows you how you can use server as storage for
 * your data like EEPROM
 *
 * Project setup in the Blynk app (not necessary):
 *   Value display on V0 in PUSH mode.
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
int uptimeCounter;

void increment() {
  uptimeCounter++;

  //storing int in V0 pin on server
  Blynk.virtualWrite(V0, uptimeCounter);
}

void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth);

  while (Blynk.connect() == false) {
    // Wait until connected
  }

  timer.setInterval(1000L, increment);
}

void loop()
{
  Blynk.run();
  timer.run();
}

// This function will run every time Blynk connection is established
BLYNK_CONNECTED() {
  //get data stored in virtual pin V0 from server
  Blynk.syncVirtual(V0);
}

// restoring counter from server
BLYNK_WRITE(V0)
{
  //restoring int value
  uptimeCounter = param.asInt();
}
