/*
 * Monitor button state using polling mechanism.
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
}

int prevState = -1;
int currState = -1;
long lastChangeTime = 0;

void checkPin()
{
  // Invert state, as button is "Active LOW"
  int state = !digitalRead(2);

  // Debounce mechanism
  long t = millis();
  if (state != prevState) {
    lastChangeTime = t;
  }
  if (t - lastChangeTime > 50) {
    if (state != currState) {
      currState = state;
      Blynk.virtualWrite(1, state);
    }
  }
  prevState = state;
}

void loop()
{
  Blynk.run();
  checkPin();
}

