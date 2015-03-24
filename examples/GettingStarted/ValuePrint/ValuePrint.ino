/*
 * You can use this sketch as a debug tool that prints all incoming values
 * sent by a widget connected to a Virtual Pin 1 in the Blynk App.
 *
 * App dashboard setup:
 *   Slider (0...100) on V1
 */

#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth);
}

// This function will be called every time
// when App writes value to Virtual Pin 1
BLYNK_WRITE(1)
{
  BLYNK_DBG_DUMP("Got a value: ", param.getBuffer(), param.getLength());
}

void loop()
{
  Blynk.run();
}

