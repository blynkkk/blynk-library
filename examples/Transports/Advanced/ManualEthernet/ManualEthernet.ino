#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>

// Auth token you get from App
char auth[] = "00000000000000000000000000000000";

IPAddress server_ip (10, 0, 0, 10);

// Mac address should be different for each device in your LAN
byte arduino_mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress arduino_ip ( 10,   0,   0,  20);
IPAddress dns_ip     (  8,   8,   8,   8);
IPAddress gateway_ip ( 10,   0,   0,   1);
IPAddress subnet_mask(255, 255, 255,   0);

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, server_ip, 8282, arduino_ip, dns_ip, gateway_ip, subnet_mask, arduino_mac);
  // Or like this:
  //Blynk.begin(auth, "server.org", 8282, arduino_ip, dns_ip, gateway_ip, subnet_mask, arduino_mac);
}

void loop()
{
  Blynk.run();
}

