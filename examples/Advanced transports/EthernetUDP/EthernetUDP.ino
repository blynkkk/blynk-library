#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernetUdp.h>

// Auth token you get from App
char auth[] = "6b291640fd1c4bf69368bf555f514bb9";

// Mac address should be different for each device in your LAN
byte arduino_mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte arduino_ip[]  = { 10, 0, 0, 20 };
byte server_ip[]   = { 10, 0, 0, 10 };

void setup()
{
  // Use DHCP
  Ethernet.begin(arduino_mac);
  // Or make program smaller my using static IP:
  //Ethernet.begin(arduino_mac, arduino_ip);

  Blynk.begin(auth);
  // Or specify server using one of those commands:
  //Blynk.begin(auth, "server.org", 8080);
  //Blynk.begin(auth, server_ip, port);
}

void loop()
{
  Blynk.run();
}

