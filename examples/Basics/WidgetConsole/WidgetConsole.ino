/*
 * You can send/receive any data using WidgetConsole object.
 *
 * App dashboard setup:
 *   Console widget on V1
 */

#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Attach virtual serial console to virtual pin 1
WidgetConsole console(1);

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth);
}

BLYNK_WRITE(1)
{
  BLYNK_LOG("Hey! I got a new message!");

  // Send it back
  console.println("I got:");
  console.write(param.getBuffer(), param.getLength());
  console.flush();
}

void loop()
{
  Blynk.run();
}

