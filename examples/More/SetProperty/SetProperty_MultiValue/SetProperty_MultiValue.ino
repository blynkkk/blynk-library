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
 * You can set predefined properties of any widget. Like color, label
 *
 * Project setup in the Blynk app:
 *   Menu Widget on V1 with 2 items
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

void loop()
{
  Blynk.run();
}

BLYNK_WRITE(V1) {

  switch (param.asInt())
  {
    case 1: {
      Serial.println("Item 1 selected");
    } break;
    case 2: {
      // If item 2 is selected, change menu items...
      BlynkParamAllocated items(128); // list length, in bytes
      items.add("New item 1");
      items.add("New item 2");
      items.add("New item 3");
      Blynk.setProperty(V1, "labels", items);

      // You can also use overloaded methods if you have less than 4 items
      //Blynk.setProperty(V1, "labels", "New item 1", "New item 2", "New item 3");

    } break;
    default:
      Serial.println("Unknown item selected");
  }

}

