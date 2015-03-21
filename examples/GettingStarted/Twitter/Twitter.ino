/*
 * Simple tweet example
 *
 * App dashboard setup:
 *   Twitter widget
 */

#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth);
  if (Blynk.connect()) {
    Blynk.tweet("Arduino tweeting!");
  }
}

void loop()
{
  Blynk.run();
}

