#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>

// Auth token you get from App
char auth[] = "00000000000000000000000000000000";

// Mac address should be different for each device in your LAN
byte arduino_mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress arduino_ip (10, 0, 0, 20);
IPAddress server_ip (10, 0, 0, 10);

void setup()
{
  // Everything except auth is optional ;)
  Blynk.begin(auth, server_ip, 8282, arduino_ip, arduino_mac);
  // Or like this:
  //Blynk.begin(auth, "server.org", 8282, arduino_ip, arduino_mac);
}

void loop()
{
  Blynk.run();
}

