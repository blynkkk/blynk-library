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

  This example shows how to fetch data using WebHook GET method

  App project setup:
    WebHook widget on V0, method: GET, url: /pin/
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
// Allow for receiving messages up to 512 bytes long
//#define BLYNK_MAX_READBYTES 512

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

BLYNK_WRITE(V0)
{
  Serial.println("WebHook data:");
  Serial.println(param.asStr());
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth);

  Blynk.virtualWrite(V0, "https://raw.githubusercontent.com/blynkkk/blynk-library/master/extras/logo.txt");

  // You can perform HTTPS requests even if your hardware alone can't handle SSL
  // Blynk  can also fetch much bigger messages,
  // if hardware has enough RAM (set BLYNK_MAX_READBYTES to 4096)
  //Blynk.virtualWrite(V0, "https://api.sunrise-sunset.org/json?lat=50.4495484&lng=30.5253873&date=2016-10-01");
}

void loop()
{
  Blynk.run();
}

