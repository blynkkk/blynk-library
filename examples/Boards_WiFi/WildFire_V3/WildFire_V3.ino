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
  This example shows how to use CC3000 on WildFire board
  to connect your project to Blynk.

  Note: You need to install Arduino IDE WildFire support:
    http://shop.wickeddevice.com/resources/wildfire/

  Note: Firmware version 1.14 or later is preferred.

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <SPI.h>
#include <WildFire.h>
#include <WildFire_CC3000.h>
#include <BlynkSimpleWildFire.h>

WildFire wildfire;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Choose wifi_sec from WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";
int wifi_sec = WLAN_SEC_WPA2;

void setup()
{
  // Debug console
  Serial.begin(9600);

  wildfire.begin();
  Blynk.begin(auth, ssid, pass, wifi_sec);
}

void loop()
{
  Blynk.run();
}

