/*
 * Monitor button state using interrupts mechanism.
 *
 * The dashboard:
 *   LED on V1
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

  // Make pin 2 default HIGH, and attach INT to our handler
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(0, checkPin, CHANGE);
}

void checkPin()
{
  // Invert state, as button is "Active LOW"
  int state = !digitalRead(2);
  Blynk.virtualWrite(1, state);
}

void loop()
{
  Blynk.run();
}

