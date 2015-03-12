#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <WiFly.h>
#include <BlynkSimpleWiFly.h>

// For this example you need WiFly_Shield library
// https://github.com/sparkfun/WiFly-Shield/tree/master/Libraries/WiFly_Shield

// Auth token you get from App
char auth[] = "00000000000000000000000000000000";

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, "ssid", "pass");
}

void loop()
{
  Blynk.run();
}

