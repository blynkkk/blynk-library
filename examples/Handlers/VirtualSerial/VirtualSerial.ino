#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>
#include <VirtualSerial.h>

// Attach virtual serial to virtual pin 1
VirtualSerial BlynkSerial(1);

void setup()
{
  Serial.begin(115200);
  Blynk.begin("00000000000000000000000000000000");
}

BLYNK_ON_WRITE(1)
{
  BLYNK_LOG("Hey! I got a new message!");

  // Send it back
  BlynkSerial.println("I got:");
  BlynkSerial.write(param.getBuffer(), param.getLength());
  BlynkSerial.flush();
}

void loop()
{
  Blynk.run();
}

