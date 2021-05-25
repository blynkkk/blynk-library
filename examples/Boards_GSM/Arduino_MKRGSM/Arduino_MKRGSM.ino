/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example shows how to use Arduino MKRGSM 1400
  to connect your project to Blynk.

  Note: This requires MKRGSM library
    from https://www.arduino.cc/en/Reference/MKRGSM

  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
//#define BLYNK_TEMPLATE_ID   "YourTemplateID"


#include <SPI.h>
#include <MKRGSM.h>
#include <BlynkSimpleMKRGSM.h>

GSMClient client;
GPRS gprs;
GSM gsmAccess;


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your SIM and GPRS credentials
// Leave empty, if missing pin, user or pass
char pin[]  = "";
char apn[]  = "YourAPN";
char user[] = "";
char pass[] = "";

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, gsmAccess, gprs, client, pin, apn, user, pass);
}

void loop()
{
  Blynk.run();
}
