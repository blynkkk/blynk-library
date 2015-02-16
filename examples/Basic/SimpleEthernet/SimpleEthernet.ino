#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>

// Auth token you get from App
char auth[] = "bfedeff2b5a1445eb6e0519a4c965b8c";

void setup()
{
  Blynk.begin(auth);
}

void loop()
{
  Blynk.run();
}

