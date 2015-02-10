#include <SPI.h>
#include <WiFi.h>
#include <BlynkSimpleWifi.h>

// Auth token you get from App
char auth[] = "6b291640fd1c4bf69368bf555f514bb9";

void setup()
{
  Blynk.begin(auth, "yourNetwork", "yourPassword");
  // Or specify server using one of those commands:
  //Blynk.begin(auth, "yourNetwork", "yourPassword", "server.org", 8080);
  //Blynk.begin(auth, "yourNetwork", "yourPassword", server_ip, port);
}

void loop()
{
  Blynk.run();
}

