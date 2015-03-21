#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <WiFi.h>
#include <BlynkSimpleWifi.h>

// Auth token you get from App
char auth[] = "YourAuthToken";

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, "yourNetwork", "yourPassword");
  // Or specify server using one of those commands:
  //Blynk.begin(auth, "yourNetwork", "yourPassword", "server.org", 8080);
  //Blynk.begin(auth, "yourNetwork", "yourPassword", server_ip, port);
}

void loop()
{
  Blynk.run();
}

