/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  This example shows you how you can use server as storage for
  your data like EEPROM

  Project setup in the Blynk app (not necessary):
    Value display on V1 in PUSH mode.
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

BlynkTimer timer;
int uptimeCounter;
String someStaticData = "SomeStaticData";

// This function will run every time Blynk connection is established
BLYNK_CONNECTED() {
  //get data stored in virtual pin V0 from server
  Blynk.syncVirtual(V0);
}

// restoring counter from server
BLYNK_WRITE(V0)
{
  //restoring int value
  uptimeCounter = param[0].asInt();
  //restoring string value
  someStaticData = param[1].asString();
}

void increment() {
  uptimeCounter++;

  //storing int and string in V0 pin on server
  Blynk.virtualWrite(V0, uptimeCounter, someStaticData);

  //updating value display with uptimeCounter
  Blynk.virtualWrite(V1, uptimeCounter);
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth);

  timer.setInterval(1000L, increment);
}

void loop()
{
  Blynk.run();
  timer.run();
}

