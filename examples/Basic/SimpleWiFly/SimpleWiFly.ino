#include <SPI.h>
#include <WiFly.h>
#include <BlynkSimpleWiFly.h>

// For this example you need WiFly_Shield library
// https://github.com/sparkfun/WiFly-Shield/tree/master/Libraries/WiFly_Shield

// Auth token you get from App
char auth[] = "bfedeff2b5a1445eb6e0519a4c965b8c";

void setup()
{
  Blynk.begin(auth, "ssid", "pass");
}

void loop()
{
  Blynk.run();
}

