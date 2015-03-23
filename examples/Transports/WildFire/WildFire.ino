/*
 * For this example you need a WildFire device,
 * and also to install "Combined Arduino Support Package".
 * Please find more information here:
 *   http://shop.wickeddevice.com/resources/wildfire
 *
 * Change WiFi ssid, pass, and Blynk auth token to run :)
 */

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

