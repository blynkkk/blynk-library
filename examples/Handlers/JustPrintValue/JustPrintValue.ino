#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>

void setup()
{
  Serial.begin(115200);
  Blynk.begin("00000000000000000000000000000000");
}

BLYNK_ON_WRITE(1)
{
  BLYNK_LOG("Got a value: %s", param[0].asStr());
}

void loop()
{
  Blynk.run();
}

