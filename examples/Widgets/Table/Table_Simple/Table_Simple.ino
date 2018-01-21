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

  You can use Table widget for logging events

  App project setup:
    Default Table widget on V1
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
int rowIndex = 0;

void sendEvent() {
  // adding 1 row to table every second
  Blynk.virtualWrite(V1, "add", rowIndex, "My Event", millis() / 1000);

  //highlighting latest added row in table
  Blynk.virtualWrite(V1, "pick", rowIndex);

  rowIndex++;
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth);

  //clean table at start
  Blynk.virtualWrite(V1, "clr");

  //run sendEvent method every second
  timer.setInterval(1000L, sendEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
}

