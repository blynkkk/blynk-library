/*
 * This just prints a value that is sent by widget
 * connected to virtual pin 1.
 * 
 * The dashboard:
 *   Slider (0...100) on V1
 */

#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>

void setup()
{
  Serial.begin(9600);
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

