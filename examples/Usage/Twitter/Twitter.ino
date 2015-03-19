/*
 * Simple tweet example
 *
 * The dashboard:
 *   Twitter widget
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
  if (Blynk.connect()) {
    Blynk.tweet("Arduino tweeting!");
  }
}

void loop()
{
  Blynk.run();
}

