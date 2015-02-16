#include <SPI.h>
#include <WiFi.h>
#include <BlynkSimpleWifi.h>

// Auth token you get from App
char auth[] = "bfedeff2b5a1445eb6e0519a4c965b8c";

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

