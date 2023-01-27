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

  You can synchronize the state of widgets with hardware states,
  even if hardware resets or looses connection temporarily

  App dashboard setup:
    Slider widget (0...1024) on V0
    Value display (0...1024) on V2
    Button widget on digital pin (connected to an LED)
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

// This function will run every time Blynk connection is established
BLYNK_CONNECTED() {
  // Request Blynk server to re-send latest values for all pins
  Blynk.syncAll();

  // You can also update individual virtual pins like this:
  //Blynk.syncVirtual(V0, V2);

  // Let's write your hardware uptime to Virtual Pin 2
  int value = millis() / 1000;
  Blynk.virtualWrite(V2, value);
}

BLYNK_WRITE(V0)
{
  // Use of syncAll() will cause this function to be called
  // Parameter holds last slider value
  int sliderValue0 = param.asInt();
}

BLYNK_WRITE(V2)
{
  // You'll get uptime value here as result of syncAll()
  int uptime = param.asInt();
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

