/*
 * You can send/receive any data using VirtualSerial object.
 *
 * The dashboard:
 *   VirtualConsole widget connected to pin V1
 */

#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>

// Attach virtual serial console to virtual pin 1
WidgetConsole console(1);

void setup()
{
  Serial.begin(9600);
  Blynk.begin("00000000000000000000000000000000");
}

BLYNK_ON_WRITE(1)
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

