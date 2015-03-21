/*
 * Rotate a servo using a slider!
 *
 * App dashboard setup:
 *   Slider (0...180) on V3
 */

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>
#include <Servo.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

Servo servo;

void setup()
{
  Blynk.begin(auth);
  servo.attach(9);
}

BLYNK_WRITE(3)
{
  servo.write(param[0].asInt());
}

void loop()
{
  Blynk.run();
}

