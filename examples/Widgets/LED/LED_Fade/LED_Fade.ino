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

  Fade using a LED widget on your phone!

  Datastream setup:
    Virtual Pin V3, type: Integer, min: 0, max: 255

  App/Web dashboard setup:
    LED widget on V3
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

WidgetLED led1(V3);

BlynkTimer timer;

// V3 LED Widget is fading
void fadeLedWidget()
{
  static int value = 0;
  static int delta = 30;
  value += delta;
  if (value > 255 || value < 0) {
    delta = -delta;
  } else {
    Serial.print("LED on V3: ");
    Serial.println(value);
    led1.setValue(value);
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN);

  timer.setInterval(300L, fadeLedWidget);
}

void loop()
{
  Blynk.run();
  timer.run();
}

