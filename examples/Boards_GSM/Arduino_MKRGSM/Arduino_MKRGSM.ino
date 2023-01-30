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
  This example shows how to use Arduino MKRGSM 1400
  to connect your project to Blynk.

  NOTE: This requires MKRGSM library
    from https://www.arduino.cc/en/Reference/MKRGSM

  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"
//#define BLYNK_AUTH_TOKEN            "YourAuthToken"


#include <SPI.h>
#include <MKRGSM.h>
#include <BlynkSimpleMKRGSM.h>

// Your SIM and GPRS credentials
// Leave empty, if missing pin, user or pass
char pin[]  = "";
char apn[]  = "YourAPN";
char user[] = "";
char pass[] = "";

GSMClient client;
GPRS gprs;
GSM gsmAccess;

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, gsmAccess, gprs, client, pin, apn, user, pass);
}

void loop()
{
  Blynk.run();
}
