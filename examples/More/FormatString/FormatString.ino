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

  You can construct and display any strings on a Value Display.

  App dashboard setup:
    Value Display widget attached to V5
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

BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5)
void sendTemperature()
{
  // Generate random temperature value 10.0 to 30.0 (for example)
  float t = float(random(100, 300)) / 10;

  // Format: 1 decimal place, add ℃
  String str = String(t, 1) + "℃";

  // Send it to the server
  Blynk.virtualWrite(V5, str);
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN);

  // Setup a function to be called every second
  timer.setInterval(1000L, sendTemperature);
}

void loop()
{
  Blynk.run();
  timer.run();
}

