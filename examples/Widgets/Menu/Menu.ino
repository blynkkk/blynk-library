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

  This example shows how to use the Menu Widget.

  Datastream setup:
    Virtual Pin V1, type: Integer, min: 1, max: 3

  App/Web dashboard setup:
    Menu widget attached to V1 (put 3 items in it)

  For Integer DS, pay attention to the Datastream range.
    Apps automatically assign values to the menu items,
    starting with the minimal datastream value.
  It's also possible to use Enum datastreams.
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

BLYNK_WRITE(V1) {
  switch (param.asInt())
  {
    case 1: // Item 1
      Serial.println("Item 1 selected");
      break;
    case 2: // Item 2
      Serial.println("Item 2 selected");
      break;
    case 3: // Item 3
      Serial.println("Item 3 selected");
      break;
    default:
      Serial.println("Unknown item selected");
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN);
}

void loop()
{
  Blynk.run();
}

