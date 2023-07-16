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

  You can set predefined properties (color, label, ...) of any widget.
  Read more: https://docs.blynk.io/en/blynk.edgent-firmware-api/widget-properties

  Datastream setup:
    Virtual Pin V1, type: Integer, min: 1, max: 3

  App dashboard setup:
    Menu Widget on V1 with 2 items: "Item 1", "Add items"
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
  int value = param.asInt();
  if (value == 1) {
    Serial.println("Item 1 selected");
  } else if (value == 2) {
    // If item 2 is selected, change menu items...
    BlynkParamAllocated items(128); // list length, in bytes
    items.add("New item 1");
    items.add("New item 2");
    items.add("New item 3");
    Blynk.setProperty(V1, "labels", items);

    // You can also use it like this:
    //Blynk.setProperty(V1, "labels", "item 1", "item 2", "item 3");

  } else {
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

