/*
 * This example shows how a custom value
 * is periodically requested by the phone App.
 * 
 * The dasboard:
 *   Value Display on V5 with Frequency 1000
 * 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>
#include <SimpleTimer.h>

// Auth token you get from App
char auth[] = "00000000000000000000000000000000";

void setup()
{
  Blynk.begin(auth);
}

BLYNK_ON_READ(5)
{
  Blynk.virtualWrite(5, millis()/1000);
}

void loop()
{
  Blynk.run();
}

