#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>

// Auth token you get from App
char auth[] = "00000000000000000000000000000000";

void setup()
{
  Blynk.begin(auth);
}

void loop()
{
  Blynk.run();
}

