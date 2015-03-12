#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>

// Auth token you get from App
char auth[] = "00000000000000000000000000000000";

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth);
}

void loop()
{
  Blynk.run();
}

