#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>

// Auth token you get from App
char auth[] = "6b291640fd1c4bf69368bf555f514bb9";

// Mac address should be different for each device in your LAN
byte arduino_mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte arduino_ip[]  = { 10, 0, 0, 20 };
byte server_ip[]   = { 10, 0, 0, 10 };

void setup()
{
  // Everything except auth is optional ;)
  Blynk.begin(auth, server_ip, 8080, arduino_mac, arduino_ip);
  // Or like this:
  //Blynk.begin(auth, "server.org", 8080, arduino_mac, arduino_ip);
}

void loop()
{
  Blynk.run();
}

