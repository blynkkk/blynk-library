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
 * This example shows how LOW/HIGH event may be triggered from
 * Blynk Server to Arduino at specific time.
 *
 * Timer widget works for ANALOG and DIGITAL pins also.
 * In this case you don't need to write code.
 * Blynk handles that for you.
 *
 * App dashboard setup:
 *   Timer widget attached to V5 and running dashboard.
 *
 **************************************************************/

#define BLYNK_PRINT Serial // Enables Serial Monitor
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h> // This part is for Ethernet stuff

char auth[] = "YourAuthToken"; // Put your Auth Token here. (see Step 3 above)

void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth);  // Here your Arduino connects to the Blynk Cloud.
}

BLYNK_WRITE(V5)
{
  //You'll get HIGH/1 at startTime and LOW/0 at stopTime.
  //this method will be triggered every day
  //until you remove widget or stop dashboard or
  //clean stop/start fields of widget
  BLYNK_LOG("Got a value: %s", param.asStr());
}

void loop()
{
  Blynk.run(); // All the Blynk Magic happens here...

  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}

