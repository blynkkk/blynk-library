/*
 * This example shows how a custom value
 * is periodically requested by the Blynk App.
 *
 * App dashboard setup:
 *   Value Display on V5 with Frequency 1000
 */

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>
#include <SimpleTimer.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  Blynk.begin(auth);
}

//
BLYNK_READ(5)
{
  Blynk.virtualWrite(5, millis()/1000);
}

void loop()
{
  Blynk.run();
}

