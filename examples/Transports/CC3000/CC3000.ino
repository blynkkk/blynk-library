/*
 * For this example you need Adafruit_CC3000_Library library:
 *   https://github.com/adafruit/Adafruit_CC3000_Library
 */

#define BLYNK_PRINT Serial
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <BlynkSimpleCC3000.h>

// Auth token you get from App
char auth[] = "00000000000000000000000000000000";

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, "ssid", "pass", WLAN_SEC_WPA2);
}

void loop()
{
  Blynk.run();
}

