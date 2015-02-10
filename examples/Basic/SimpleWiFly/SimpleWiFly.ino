#include <SPI.h>
#include <WiFly.h>
#include <BlynkSimpleWiFly.h>

// For this example you need WiFly_Shield library
// https://github.com/sparkfun/WiFly-Shield/tree/master/Libraries/WiFly_Shield

// Auth token you get from App
char auth[] = "6b291640fd1c4bf69368bf555f514bb9";

void setup()
{
  Blynk.begin(auth, "ssid", "pass");
}

void loop()
{
  Blynk.run();
}

