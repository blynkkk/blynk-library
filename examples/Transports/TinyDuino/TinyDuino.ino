/*
 * Change WiFi ssid, pass, and Blynk auth token to run :)
 */

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <BlynkSimpleTinyDuino.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, "ssid", "pass", WLAN_SEC_WPA2);
}

void loop()
{
  Blynk.run();
}

