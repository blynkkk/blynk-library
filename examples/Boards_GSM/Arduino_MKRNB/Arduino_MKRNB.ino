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
  This example shows how to use Arduino MKRNB 1500
  to connect your project to Blynk.
  Note: This requires MKRGSM library
    from https://www.arduino.cc/en/Reference/MKRNB
  Feel free to apply it to any other example. It's simple!
 *************************************************************/
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <MKRNB.h>
#include <BlynkSimpleMKRNB.h>

NBClient client;
GPRS gprs;
NB nbAccess;
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";
// Your SIM credential
// Leave empty, if missing pin
char pin[]  = "";

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, nbAccess, gprs, client , pin);
}
void loop()
{
  Blynk.run();
}
