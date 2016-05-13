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
 * This example shows how to use CC3000 on WildFire board
 * to connect your project to Blynk.
 * Feel free to apply it to any other example. It's simple!
 *
 * For this example you need a WildFire device,
 * and also to install "Combined Arduino Support Package".
 * Please find more information here:
 *   http://shop.wickeddevice.com/resources/wildfire
 *
 * Note: Firmware version 1.14 or later is preferred.
 *
 * Change WiFi ssid, pass, and Blynk auth token to run :)
 *
 **************************************************************/

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <WildFire.h>
#include <WildFire_CC3000.h>
#include <BlynkSimpleWildFire.h>

WildFire wildfire;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  Serial.begin(9600);
  wildfire.begin();
  Blynk.begin(auth, "ssid", "pass", WLAN_SEC_WPA2);
}

void loop()
{
  Blynk.run();
}

