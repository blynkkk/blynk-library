/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  Output any data on Map widget!

  Datastream setup:
    Virtual Pin V6, type: String

  App dashboard setup:
    Map widget on V6

  NOTE: This is for the App Map widget only.
  The Web Dashboard has different functionality. Read More:
    https://docs.blynk.io/en/blynk.console/widgets-console/map
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"
//#define BLYNK_AUTH_TOKEN            "YourAuthToken"


#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

WidgetMap myMap(V6);

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN);

  // If you want to remove all points:
  //myMap.clear();

  int index = 0;
  double lat = 50.4495378;
  double lon = 30.5251447;
  myMap.location(index, lat, lon, "Monument of Independence");
}

void loop()
{
  Blynk.run();
}

