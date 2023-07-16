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

  Datastream setup:
    Virtual Pin V1, type: String

  App dashboard setup:
    Time Input widget on V1
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

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN);
}

//as soon as connected update TimeInput widget state
BLYNK_CONNECTED() {
  //seconds from the start of a day. 0 - min, 86399 - max
  int startAt = 5 * 60; //00:05

  //seconds from the start of a day. 0 - min, 86399 - max
  int stopAt = (60 + 5) * 60; //01:05

  //timezone
  //full list of supported timezones could be found here
  //https://www.mkyong.com/java/java-display-list-of-timezone-with-gmt/
  char tz[] = "Europe/Kiev";

  Blynk.virtualWrite(V1, startAt, stopAt, tz);

  //you may also pass day
  //char days[] = "1"; //Monday
  //Blynk.virtualWrite(V1, startAt, stopAt, tz, days);

  //or days
  //char days[] = "1,2,3"; //Monday, Tuesday, Wednesday
  //Blynk.virtualWrite(V1, startAt, stopAt, tz, days);
}

void loop()
{
  Blynk.run();
}

